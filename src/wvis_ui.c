/* wvis_ui.c */
/*
 * ggobi
 * Copyright (C) AT&T, Duncan Temple Lang, Dianne Cook 1999-2005
 *
 * ggobi is free software; you may use, redistribute, and/or modify it
 * under the terms of the Common Public License, which is distributed
 * with the source code and displayed on the ggobi web site, 
 * www.ggobi.org.  For more information, contact the authors:
 *
 *   Deborah F. Swayne   dfs@research.att.com
 *   Di Cook             dicook@iastate.edu
 *   Duncan Temple Lang  duncan@wald.ucdavis.edu
 *   Andreas Buja        andreas.buja@wharton.upenn.edu
*/

#include <string.h>
#include <stdlib.h>

#include <gtk/gtk.h>
#include "vars.h"
#include "externs.h"
#include "utils_ui.h"

#include "colorscheme.h"

static gint xmargin = 20;
static gint ymargin = 20;

static void bin_counts_reset (gint jvar, GGobiStage *d, GGobiSession *gg);
static void selection_made_cb (GtkTreeSelection *tree_sel, GGobiSession *gg);

/*-------------------------------------------------------------------------*/

static void
bin_counts_reset (gint jvar, GGobiStage *d, GGobiSession *gg)
{
  gint k, m;
  gdouble val;
  GGobiVariable *var;
  gdouble min, max;
  colorschemed *scheme = gg->activeColorScheme;

  if (jvar == -1)
    return;

  var = ggobi_stage_get_variable(d, jvar);
  min = ggobi_variable_get_min(var);
  max = ggobi_variable_get_max(var);

  for (k=0; k<gg->wvis.npct; k++)
    gg->wvis.n[k] = 0;

  for (m=0; m<d->n_rows; m++) {
    for (k=0; k<scheme->n; k++) {
       val = min + gg->wvis.pct[k] * (max - min);
      if (ggobi_stage_get_raw_value(d, m, jvar) <= val) {
        gg->wvis.n[k]++;
        break;
      }
    }
  }

}

static void
record_colors_reset (gint selected_var, GGobiStage *d, GGobiSession *gg)
{
  gint k, m;
  gint nd = g_slist_length(gg->d);
  GGobiVariable *var;
  gdouble min, max, val;
  colorschemed *scheme = gg->activeColorScheme;

  if (selected_var < 0)
    return;

  var = ggobi_stage_get_variable(d, selected_var);
  min = ggobi_variable_get_min(var);
  max = ggobi_variable_get_max(var);

  GGOBI_STAGE_ATTR_INIT_ALL(d);  
  for (m=0; m<d->n_rows; m++) {
    for (k=0; k<scheme->n; k++) {
      val = min + gg->wvis.pct[k] * (max - min);
      if (ggobi_stage_get_raw_value(d, m, selected_var) <= val) {
        GGOBI_STAGE_SET_ATTR_COLOR(d, m, k, ATTR_SET_PERSISTENT);
        break;
      }
    }
    if (nd > 1 && !gg->linkby_cv)
      brush_all_matching_id (d, m, true, br_color, ATTR_SET_PERSISTENT);
  }
}


/*-- called when closed from the close button --*/
static void close_btn_cb (GtkWidget *w, GGobiSession *gg) {
  gtk_widget_hide (gg->wvis.window);
}
/*-- called when closed from the window manager --*/
static void
close_wmgr_cb (GtkWidget *w, GdkEventButton *event, GGobiSession *gg)
{
  gtk_widget_hide (gg->wvis.window);
}

/*
 * Use the horizontal position of the mouse to move the nearest
 * boundary
*/
static gint
motion_notify_cb (GtkWidget *w, GdkEventMotion *event, GGobiSession *gg)
{
  GdkModifierType state;
  icoords pos;
  gboolean rval = false;
  gdouble val;

  GtkWidget *tree_view = get_tree_view_from_object (G_OBJECT (w));
  GGobiStage *d = NULL;
  gint selected_var = -1;

  icoords *mousepos = &gg->wvis.mousepos;
  gint color = gg->wvis.nearest_color;

  if(tree_view) {
    d = (GGobiStage *) g_object_get_data(G_OBJECT (tree_view), "datad");
    selected_var = get_one_selection_from_tree_view (tree_view, d);
  }

  gdk_window_get_pointer (w->window, &pos.x, &pos.y, &state);

  if (pos.x != mousepos->x) {
    val = (gdouble) (pos.x - xmargin) /
          (gdouble) (w->allocation.width - 2*xmargin);

    /*-- don't allow it to cross its neighbors' boundaries --*/
    if ((color == 0 && val <= gg->wvis.pct[color+1] && val >= 0) ||
        (color == gg->wvis.npct-1 && val >= gg->wvis.pct[color+1]) ||
        (val >= gg->wvis.pct[color-1] && val <= gg->wvis.pct[color+1]))
    {
      gg->wvis.pct[color] = val;

      if (selected_var != -1 && selected_var < d->n_cols)
        bin_counts_reset (selected_var, d, gg);

      g_signal_emit_by_name(G_OBJECT (w), "expose_event",
        (gpointer) gg, (gpointer) &rval);

      if (gg->wvis.update_method == WVIS_UPDATE_CONTINUOUSLY) {
        record_colors_reset (selected_var, d, gg);
        clusters_set(d);
        displays_plot (NULL, FULL, gg);
      }
    }
  }

  mousepos->x = pos.x;  

  return true;
}

/*-- when the button is pressed, listen for motion notify events --*/
static gint
button_press_cb (GtkWidget *w, GdkEventButton *event, GGobiSession *gg)
{
  GdkModifierType state;
  icoords pos;
  gint k, x, y, nearest = -1, d;
  gint dist = w->allocation.width*w->allocation.width +
              w->allocation.height*w->allocation.height;
  colorschemed *scheme = gg->activeColorScheme;

  gdouble *pct = gg->wvis.pct;
  gint *nearest_color = &gg->wvis.nearest_color;
  gint hgt;

  hgt = (w->allocation.height - 2*ymargin) / (scheme->n - 1);

  gdk_window_get_pointer (w->window, &pos.x, &pos.y, &state);

  /*-- find nearest slider --*/
  y = ymargin + 10;
  for (k=0; k<scheme->n - 1; k++) {
    x = xmargin + pct[k] * (w->allocation.width - 2*xmargin);
    d = (pos.x-x)*(pos.x-x) + (pos.y-y)*(pos.y-y);
    if (d < 100 && d < dist) {
      nearest = k;
      dist = d;
    }
    y += hgt;
  }

  *nearest_color = nearest;

  if (*nearest_color != -1) {
    gg->wvis.motion_notify_id = g_signal_connect (G_OBJECT (w),
      "motion_notify_event", G_CALLBACK(motion_notify_cb), (gpointer) gg);
  }

  return true;
}

static gint
button_release_cb (GtkWidget *w, GdkEventButton *event, GGobiSession *gg)
{
  GtkWidget *tree_view = get_tree_view_from_object (G_OBJECT (w));
  GGobiStage *d = NULL; 
  gint selected_var = -1;

  if(tree_view) {
      d = (GGobiStage *) g_object_get_data(G_OBJECT (tree_view), "datad");
      selected_var = get_one_selection_from_tree_view (tree_view, d);
  }

  if (gg->wvis.motion_notify_id) {
    g_signal_handler_disconnect (G_OBJECT (w), gg->wvis.motion_notify_id);
    gg->wvis.motion_notify_id = 0;
  }

  if (selected_var >= 0 && selected_var <= d->n_cols) {
    record_colors_reset (selected_var, d, gg);
    clusters_set(d);
    displays_plot (NULL, FULL, gg);
  }

  return true;
}

static gint
da_configure_cb (GtkWidget *w, GdkEventConfigure *event, GGobiSession *gg)
{
  /*-- Create new backing pixmaps of the appropriate size --*/
  if (gg->wvis.pix != NULL)
    gdk_pixmap_unref (gg->wvis.pix);
  gg->wvis.pix = gdk_pixmap_new (w->window,
    w->allocation.width, w->allocation.height, -1);

  gtk_widget_queue_draw (w);

  return false;
}

/*
 * Set the bin boundaries (the values of wvis.pct[]) in one
 * of two ways:  simply dividing the range of the data into
 * scheme->n equal-sized pieces, or attempting to set the regions
 * such that they contain equal numbers of points.
*/ 
static void
bin_boundaries_set (gint selected_var, GGobiStage *d, GGobiSession *gg)
{
  gint k;

  if (gg->wvis.binning_method == WVIS_EQUAL_WIDTH_BINS || selected_var == -1) {
    /*
     * These numbers are the upper boundaries of each interval.
     * By default, they start at .1 and end at 1.0.
    */
    for (k=0; k<gg->wvis.npct; k++) {
      gg->wvis.pct[k] = (gdouble) (k+1) /  (gdouble) gg->wvis.npct;
      gg->wvis.n[k] = 0;
    }
  } else if (gg->wvis.binning_method == WVIS_EQUAL_COUNT_BINS) {
    gint i, m;
    gdouble min, max, range, midpt;
    GGobiVariable *var = ggobi_stage_get_variable(d, selected_var);
    gint ngroups = gg->wvis.npct;
    gint groupsize = (gint) (d->n_rows / ngroups);
    paird *pairs = (paird *) g_malloc (d->n_rows * sizeof (paird));

    min = ggobi_variable_get_min(var);
    max = ggobi_variable_get_max(var);
    range = max - min;

    /*-- sort the selected variable --*/
    for (i=0; i<d->n_rows; i++) {
      pairs[i].f = ggobi_stage_get_raw_value(d, i, selected_var);
      pairs[i].indx = i;
    }
    qsort ((gchar *) pairs, d->n_rows, sizeof (paird), pcompare);

    /*
     * determine the boundaries that will result in equal-sized
     * groups (as well as the data permits)
     *
     * This seems to do the right thing except in one case:  the
     * variable is categorical with the number of categories less
     * than the number of groups, and the categories are not of
     * equal size.  It does something legal, but not ideal, since
     * the number of groups it identifies may be too small.
    */
    /*-- initialize the boundaries --*/
    for (k=0; k<ngroups; k++)
      gg->wvis.pct[k] = 1.0;
    i = 0;
    for (k=0; k<ngroups-1; k++) {  /*-- no need to figure out the last one --*/
      m = 0;
      while (m < groupsize || i == 0 || pairs[i].f == pairs[i-1].f) {
        m++;
        i++;
        if (i == d->n_rows-1) 
          break;
      }
      midpt = (i == d->n_rows - 1) ?
        max : pairs[i].f + (pairs[i+1].f - pairs[i].f) / 2 ;
      gg->wvis.pct[k] = (midpt - min) / range;
      if (i == d->n_rows-1) 
        break;
    }

    g_free (pairs);
  }
}

static void binning_method_cb (GtkWidget *w, GGobiSession *gg)
{
  gboolean rval = false;
  gg->wvis.binning_method = gtk_combo_box_get_active(GTK_COMBO_BOX(w));

  gg->wvis.npct = 0;  /*-- force bin_boundaries_set to be called --*/
  g_signal_emit_by_name(G_OBJECT (gg->wvis.da), "expose_event",
    (gpointer) gg, (gpointer) &rval);
}

static void update_method_cb (GtkWidget *w, GGobiSession *gg)
{
  gg->wvis.update_method = gtk_combo_box_get_active(GTK_COMBO_BOX(w));
}

static void alloc_pct (GGobiSession *gg)
{
  colorschemed *scheme = gg->activeColorScheme;
  if (gg->wvis.npct != scheme->n) {
    gg->wvis.npct = scheme->n;
    gg->wvis.pct = (gdouble *) g_realloc (gg->wvis.pct,
                                         gg->wvis.npct * sizeof (gdouble));
    gg->wvis.n = (gint *) g_realloc (gg->wvis.n,
                                     gg->wvis.npct * sizeof (gint));
  }
}

static void
da_expose_cb (GtkWidget *w, GdkEventExpose *event, GGobiSession *gg)
{
  gint height = w->allocation.height - 2*ymargin;
  gint x0, x1, k, hgt;
  gint x = xmargin;
  gint y = ymargin;
  gdouble diff;
  GGobiVariable *var;
  colorschemed *scheme = gg->activeColorScheme;

  GtkWidget *tree_view = get_tree_view_from_object (G_OBJECT (w));
  GGobiStage *d = NULL;
  gint selected_var = -1;

  GtkWidget *da = gg->wvis.da;
  GdkPixmap *pix = gg->wvis.pix;

  if(tree_view) {
    d = (GGobiStage *) g_object_get_data(G_OBJECT (tree_view), "datad");
    selected_var = get_one_selection_from_tree_view (tree_view, d);
    if (selected_var == -1) {
      selected_var = 0;
      select_tree_view_row (tree_view, selected_var);
    }
  }

  if (gg->wvis.GC == NULL)
    gg->wvis.GC = gdk_gc_new (w->window);

  hgt = height / (scheme->n - 1);

  if (gg->wvis.npct != scheme->n) {
    gg->wvis.npct = scheme->n;
    gg->wvis.pct = (gdouble *) g_realloc (gg->wvis.pct,
                                         gg->wvis.npct * sizeof (gdouble));
    gg->wvis.n = (gint *) g_realloc (gg->wvis.n,
                                     gg->wvis.npct * sizeof (gint));
    bin_boundaries_set (selected_var, d, gg);
    bin_counts_reset (selected_var, d, gg);
  }

  /*-- clear the pixmap --*/
  gdk_gc_set_foreground (gg->wvis.GC, &scheme->rgb_bg);
  gdk_draw_rectangle (pix, gg->wvis.GC, TRUE,
                      0, 0, w->allocation.width, w->allocation.height);


  /*-- draw the color bars --*/
  x0 = xmargin;
  for (k=0; k<scheme->n; k++) {
    x1 = xmargin + gg->wvis.pct[k] * (w->allocation.width - 2*xmargin);
    gdk_gc_set_foreground (gg->wvis.GC, &scheme->rgb[k]);
    gdk_draw_rectangle (pix, gg->wvis.GC,
                        TRUE, x0, ymargin, x1 - x0, height);
    x0 = x1;
  }

  /*-- draw the horizontal lines --*/
  x0 = xmargin; y = ymargin + 10;
  x1 = xmargin + (w->allocation.width - 2*xmargin) - 1;
  gdk_gc_set_foreground (gg->wvis.GC, &gg->mediumgray);
  for (k=0; k<scheme->n-1; k++) {
    gdk_draw_line (pix, gg->wvis.GC, x0, y, x1, y);
    y += hgt;
  }

  /*-- draw rectangles, 20 x 10 --*/
  y = ymargin + 10;
  for (k=0; k<scheme->n-1; k++) {
    x = xmargin + gg->wvis.pct[k] * (w->allocation.width - 2*xmargin);
    draw_3drectangle (w, pix, x, y, 20, 10, gg);
    y += hgt;
  }

  /*-- add the variable limits in the top margin --*/
  if (d && selected_var != -1) {
    gdouble min, max;
    gdouble val;
    gchar *str;
    PangoRectangle rect;
    PangoLayout *layout = gtk_widget_create_pango_layout(da, NULL);

    var = ggobi_stage_get_variable(d, selected_var);
    if (var) {
      min = ggobi_variable_get_min(var);
      max = ggobi_variable_get_max(var);

      gdk_gc_set_foreground (gg->wvis.GC, &scheme->rgb_accent);
      y = ymargin;
      for (k=0; k<scheme->n-1; k++) {

        val = min + gg->wvis.pct[k] * (max - min);
        str = g_strdup_printf ("%3.3g", val);
        layout_text(layout, str, &rect);
        x = xmargin + gg->wvis.pct[k] * (w->allocation.width - 2*xmargin);
        gdk_draw_layout(pix, gg->wvis.GC, x - rect.width/2, y - 2 - rect.height, layout);
        g_free (str);
      }

      /*-- ... and the counts in the bottom margin --*/
      for (k=0; k<scheme->n; k++) {
        val = min + gg->wvis.pct[k] * (max - min);
        str = g_strdup_printf ("%d", gg->wvis.n[k]);
        layout_text(layout, str, &rect);
        x = xmargin + gg->wvis.pct[k] * (w->allocation.width - 2*xmargin);
        diff = (k == 0) ? gg->wvis.pct[k] : gg->wvis.pct[k]-gg->wvis.pct[k-1]; 
        x -= diff/2 * (w->allocation.width - 2*xmargin);
        gdk_draw_layout(pix, gg->wvis.GC, 
          x - rect.width/2,
          (w->allocation.height - ymargin) + 2,
          layout);
        g_free (str);
      }
    }
    g_object_unref(G_OBJECT(layout));
  }

  gdk_draw_pixmap (w->window, gg->wvis.GC, pix,
                   0, 0, 0, 0,
                   w->allocation.width,
                   w->allocation.height);
}

void
selection_made_cb (GtkTreeSelection *tree_sel, GGobiSession *gg)
{
  gboolean rval = false;
  GtkTreeView *tree_view = gtk_tree_selection_get_tree_view(tree_sel);
  GGobiStage *d = (GGobiStage *) g_object_get_data(G_OBJECT (tree_view), "datad");
  GtkWidget *btn;
  gint row;

  row = tree_selection_get_selected_row(tree_sel);
  if (row == -1)
    return;
  
  bin_boundaries_set (row, d, gg);  /*-- in case the method changed --*/
  bin_counts_reset (row, d, gg);
  g_signal_emit_by_name(G_OBJECT (gg->wvis.da), "expose_event",
    (gpointer) gg, (gpointer) &rval);

  /*-- get the apply button, make it sensitive --*/
  btn = widget_find_by_name (gg->wvis.window, "WVIS:apply");
  if (btn)
    gtk_widget_set_sensitive (btn, true);
}


static void scale_apply_cb (GtkWidget *w, GGobiSession* gg)
{
  GtkWidget *tree_view = get_tree_view_from_object (G_OBJECT (w));
  GGobiStage *d = (GGobiStage *) g_object_get_data(G_OBJECT (tree_view), "datad");
  gint selected_var = get_one_selection_from_tree_view (tree_view, d);
  colorschemed *scheme = gg->activeColorScheme;

  if (d && selected_var != -1) {
    gboolean rval = false;

    record_colors_reset (selected_var, d, gg);
    clusters_set(d);

    /*-- before calling displays_plot, reset brushing color if needed --*/
    if (gg->color_id >= scheme->n) gg->color_id = scheme->n - 1;

    displays_plot (NULL, FULL, gg);

    bin_counts_reset (selected_var, d, gg);
    g_signal_emit_by_name(G_OBJECT (gg->wvis.da), "expose_event",
      (gpointer) gg, (gpointer) &rval);

    symbol_window_redraw (gg);
    cluster_table_update (d, gg);
  }
}

void
wvis_window_open (GGobiSession *gg) 
{
  GtkWidget *vbox, *hb;
  GtkWidget *notebook = NULL;
  GtkWidget *btn, *opt;
  static gchar *const binning_method_lbl[] = {
    "Constant bin width",
    "Constant bin count (approx)"};
  static gchar *const update_method_lbl[] = {
    "Update on mouse release",
    "Update continuously"};

  alloc_pct (gg);

  if (gg->wvis.window == NULL) {

    gg->wvis.window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (gg->wvis.window),
      "Automatic Brushing by Variable");
    g_signal_connect (G_OBJECT (gg->wvis.window),
      "delete_event", G_CALLBACK (close_wmgr_cb), gg);

    vbox = gtk_vbox_new (false, 0);
    gtk_container_set_border_width (GTK_CONTAINER (vbox), 5);
    gtk_box_set_spacing (GTK_BOX(vbox), 5);
    gtk_container_add (GTK_CONTAINER (gg->wvis.window), vbox);    

    /* Create a notebook, set the position of the tabs */
    notebook = create_variable_notebook (vbox, GTK_SELECTION_SINGLE,
      GGOBI_VARIABLE_ALL_VARTYPES, all_datatypes,
      G_CALLBACK(selection_made_cb), NULL, gg);
    gtk_widget_set_sensitive(notebook, true);


    /*-- now we get fancy:  draw the scale, with glyphs and colors --*/
    gg->wvis.da = gtk_drawing_area_new ();
    gtk_widget_set_double_buffered(gg->wvis.da, false);
    gtk_widget_set_size_request (GTK_WIDGET (gg->wvis.da), 400, 200);
    g_object_set_data(G_OBJECT (gg->wvis.da), "notebook", notebook);
    gtk_box_pack_start (GTK_BOX (vbox), gg->wvis.da, false, false, 0);

    g_signal_connect (G_OBJECT (gg->wvis.da),
                        "configure_event",
                        G_CALLBACK(da_configure_cb),
                        (gpointer) gg);
    g_signal_connect (G_OBJECT (gg->wvis.da),
                        "expose_event",
                        G_CALLBACK(da_expose_cb),
                        (gpointer) gg);
    g_signal_connect (G_OBJECT (gg->wvis.da),
                        "button_press_event",
                        G_CALLBACK(button_press_cb),
                        (gpointer) gg);
    g_signal_connect (G_OBJECT (gg->wvis.da),
                        "button_release_event",
                        G_CALLBACK(button_release_cb),
                        (gpointer) gg);

    gtk_widget_set_events (gg->wvis.da, GDK_EXPOSURE_MASK
               | GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK
               | GDK_POINTER_MOTION_MASK | GDK_POINTER_MOTION_HINT_MASK);

    /*-- hbox to hold the options --*/
    hb = gtk_hbox_new (false, 2);
    gtk_box_pack_start (GTK_BOX (vbox), hb, false, false, 0);

    /*-- option menu for choosing the method of binning --*/
    opt = gtk_combo_box_new_text ();
    gtk_widget_set_name (opt, "WVIS:binning_method");
    gtk_tooltips_set_tip (GTK_TOOLTIPS (gg->tips), opt,
      "Select a binning method", NULL);
    gtk_box_pack_start (GTK_BOX (hb), opt,
      false, false, 0);
    populate_combo_box (opt, (gchar**) binning_method_lbl, G_N_ELEMENTS(binning_method_lbl),
      G_CALLBACK(binning_method_cb), gg);

    /*-- option menu for choosing the method of updating --*/
    /* This should be a checkbox, I think ... */
    opt = gtk_combo_box_new_text ();
    gtk_tooltips_set_tip (GTK_TOOLTIPS (gg->tips), opt,
      "How to update the displays in response to movements of the sliders",
      NULL);
    gtk_box_pack_start (GTK_BOX (hb), opt, true, true, 0);
    populate_combo_box (opt, (gchar**) update_method_lbl, G_N_ELEMENTS(update_method_lbl),
      G_CALLBACK(update_method_cb), gg);

    /*-- hbox for buttons --*/
    gtk_box_pack_start (GTK_BOX (vbox), gtk_hseparator_new(),
      false, true, 2);
    hb = gtk_hbox_new (false, 2);
    gtk_box_pack_start (GTK_BOX (vbox), hb, false, false, 0);

    /* Apply button */
    btn = gtk_button_new_from_stock (GTK_STOCK_APPLY);
    g_object_set_data(G_OBJECT (btn), "notebook", notebook);
    gtk_tooltips_set_tip (GTK_TOOLTIPS (gg->tips), btn,
      "Apply the color scale, using the values of the variable selected in the list above",
      NULL);
    gtk_box_pack_start (GTK_BOX (hb), btn, true, true, 1);
    g_signal_connect (G_OBJECT (btn), "clicked",
                        G_CALLBACK (scale_apply_cb), gg);
    gtk_widget_set_name (btn, "WVIS:apply");



    btn = gtk_button_new_from_stock (GTK_STOCK_CLOSE);
    gtk_tooltips_set_tip (GTK_TOOLTIPS (gg->tips), btn,
      "Close the window", NULL);
    gtk_box_pack_start (GTK_BOX (hb), btn, true, false, 2);
    g_signal_connect (G_OBJECT (btn), "clicked",
                        G_CALLBACK (close_btn_cb), gg);
  }

  gtk_widget_show_all (gg->wvis.window);
  gdk_window_raise (gg->wvis.window->window);
}

