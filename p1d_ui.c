/*-- p1d_ui.c --*/
/*
    This software may only be used by you under license from AT&T Corp.
    ("AT&T").  A copy of AT&T's Source Code Agreement is available at
    AT&T's Internet website having the URL:
    <http://www.research.att.com/areas/stat/ggobi/license.html>
    If you received this software without first entering into a license
    with AT&T, you have an infringing copy of this software and cannot use
    it without violating AT&T's intellectual property rights.
*/

#ifdef USE_STRINGS_H
#include <strings.h>
#endif

#include <gtk/gtk.h>
#include "vars.h"
#include "externs.h"

static gchar *type_lbl[] = {"Texturing", "ASH"};

static void type_cb (GtkWidget *w, gpointer cbd)
{
  ggobid *gg = GGobiFromWidget(w, true);
  cpaneld *cpanel = &gg->current_display->cpanel;
  cpanel->p1d.type = GPOINTER_TO_INT (cbd);

  display_tailpipe (gg->current_display, FULL, gg);
}

static void ASH_add_lines_cb (GtkToggleButton *button, ggobid *gg)
{
  cpaneld *cpanel = &gg->current_display->cpanel;
  cpanel->p1d.ASH_add_lines_p = button->active;
  splot_redraw (gg->current_splot, FULL, gg);
}

static void ash_smoothness_cb (GtkAdjustment *adj, ggobid *gg) 
{
  cpaneld *cpanel = &gg->current_display->cpanel;

  /*-- adj->value ranges from .01 to .5; min value for nASHes = 1 --*/
  cpanel->p1d.nASHes = (gint)
    ((gfloat) cpanel->p1d.nbins * (adj->value / 2.0));

  if (cpanel->p1d.type == ASH)
    display_tailpipe (gg->current_display, FULL, gg);
}

/*--------------------------------------------------------------------*/
/*                           Cycling                                  */
/*--------------------------------------------------------------------*/

void
p1d_cycle_activate (gboolean state, cpaneld *cpanel, ggobid *gg)
{
  if (state) {
    gg->p1d.cycle_id = gtk_timeout_add (cpanel->p1d.cycle_delay,
      (GtkFunction) p1dcycle_func, (gpointer) gg);
    cpanel->p1d.cycle_p = true;
  } else {
    if (gg->p1d.cycle_id) {
      gtk_timeout_remove (gg->p1d.cycle_id);
      gg->p1d.cycle_id = 0;
      cpanel->p1d.cycle_p = false;
    }
  }
}

static void cycle_cb (GtkToggleButton *button, ggobid* gg)
{
  displayd *display = gg->current_display;
  cpaneld *cpanel = &display->cpanel;

  cpanel->p1d.cycle_p = button->active;
  p1d_cycle_activate (cpanel->p1d.cycle_p, cpanel, gg);
}
static void cycle_speed_cb (GtkAdjustment *adj, ggobid *gg)
{
  displayd *display = gg->current_display;
  cpaneld *cpanel = &display->cpanel;

  cpanel->p1d.cycle_delay = -1 * (guint32) adj->value;
  if (cpanel->p1d.cycle_p) {
    gtk_timeout_remove (gg->p1d.cycle_id);
    gg->p1d.cycle_id = gtk_timeout_add (cpanel->p1d.cycle_delay,
    (GtkFunction) p1dcycle_func, (gpointer) gg);
  }
}

static void chdir_cb (GtkButton *button, ggobid *gg)
{
  displayd *display = gg->current_display;
  cpaneld *cpanel = &display->cpanel;

  cpanel->p1d.cycle_dir = -1 * cpanel->p1d.cycle_dir;
}

/*--------------------------------------------------------------------*/
/*      Handling keyboard and mouse events in the plot window         */
/*--------------------------------------------------------------------*/

static gint
key_press_cb (GtkWidget *w, GdkEventKey *event, splotd *sp)
{
  ggobid *gg = GGobiFromSPlot(sp);
  cpaneld *cpanel = &gg->current_display->cpanel;
  
/*-- add a key_press_cb in each mode, and let it begin with these lines --*/
  if (splot_event_handled (w, event, cpanel, sp, gg))
    return true;

  /*-- insert mode-specific key presses (if any) here --*/

  return true;
}

void
p1d_event_handlers_toggle (splotd *sp, gboolean state) {
  displayd *display = (displayd *) sp->displayptr;

  if (state == on) {
    sp->key_press_id = gtk_signal_connect (GTK_OBJECT (display->window),
                                           "key_press_event",
                                           (GtkSignalFunc) key_press_cb,
                                           (gpointer) sp);
  } else {
    disconnect_key_press_signal (sp);
  }
}

/*--------------------------------------------------------------------*/

void
cpanel_p1dplot_make (ggobid *gg) {
  GtkWidget *frame, *framevb, *tgl, *btn, *vbox, *vb, *opt;
  GtkWidget *sbar;
  GtkObject *adj;
  
  gg->control_panel[P1PLOT] = gtk_vbox_new (false, VBOX_SPACING);
  gtk_container_set_border_width (GTK_CONTAINER (gg->control_panel[P1PLOT]), 5);

 /*-- option menu --*/
  opt = gtk_option_menu_new ();
  gtk_widget_set_name (opt, "P1PLOT:type_option_menu");
  gtk_tooltips_set_tip (GTK_TOOLTIPS (gg->tips), opt,
    "Display either textured dot plots or average shifted histograms", NULL);
  gtk_box_pack_start (GTK_BOX (gg->control_panel[P1PLOT]),
                      opt, false, false, 0);
  populate_option_menu (opt, type_lbl,
                        sizeof (type_lbl) / sizeof (gchar *),
                        (GtkSignalFunc) type_cb, gg);

  /*-- frame around ASH parameters --*/
  frame = gtk_frame_new ("ASH parameters");
  gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_ETCHED_OUT);
  gtk_box_pack_start (GTK_BOX (gg->control_panel[P1PLOT]), frame,
    false, false, 3);

  framevb = gtk_vbox_new (false, VBOX_SPACING);
  gtk_container_set_border_width (GTK_CONTAINER (framevb), 4);
  gtk_container_add (GTK_CONTAINER (frame), framevb);

  /*-- ASH line segments --*/
  btn = gtk_check_button_new_with_label ("ASH: add lines");
  gtk_widget_set_name (btn, "P1PLOT:ASH_add_lines");
  gtk_tooltips_set_tip (GTK_TOOLTIPS (gg->tips), btn,
    "When displaying ASHes, add lines connecting each point to the baseline.",
    NULL);
  /*-- cpanel may not be available, so initialize this to false --*/
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (btn), false);
  gtk_signal_connect (GTK_OBJECT (btn), "toggled",
    GTK_SIGNAL_FUNC (ASH_add_lines_cb), (gpointer) gg);
  gtk_box_pack_start (GTK_BOX (framevb), btn,
    false, false, 0);

  /*-- ASH smoothness --*/
  vbox = gtk_vbox_new (false, 0);
  gtk_box_pack_start (GTK_BOX (framevb), vbox, false, false, 0);

  gtk_box_pack_start (GTK_BOX (vbox), gtk_label_new ("ASH smoothness:"),
    false, false, 0);

  /*-- value, lower, upper, step --*/
  adj = gtk_adjustment_new (0.19, 0.02, 0.5, 0.01, .01, 0.0);
  gtk_signal_connect (GTK_OBJECT (adj), "value_changed",
                      GTK_SIGNAL_FUNC (ash_smoothness_cb), gg);

  sbar = gtk_hscale_new (GTK_ADJUSTMENT (adj));
  gtk_widget_set_name (sbar, "P1PLOT:ASH_smooth");
  gtk_tooltips_set_tip (GTK_TOOLTIPS (gg->tips), sbar,
    "Adjust ASH smoothness", NULL);
  gtk_range_set_update_policy (GTK_RANGE (sbar), GTK_UPDATE_CONTINUOUS);
  gtk_scale_set_value_pos (GTK_SCALE (sbar), GTK_POS_BOTTOM);
  gtk_scale_set_digits (GTK_SCALE (sbar), 2);

  gtk_box_pack_start (GTK_BOX (vbox), sbar, false, false, 1);



/*
 * Cycling controls
*/

  frame = gtk_frame_new ("Plot cycling");
  gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_ETCHED_OUT);
  gtk_box_pack_start (GTK_BOX (gg->control_panel[P1PLOT]), frame,
    false, false, 3);

  vb = gtk_vbox_new (false, VBOX_SPACING);
  gtk_container_set_border_width (GTK_CONTAINER (vb), 4);
  gtk_container_add (GTK_CONTAINER (frame), vb);

  tgl = gtk_check_button_new_with_label ("Cycle");
  gtk_widget_set_name (tgl, "P1PLOT:cycle_toggle");
  gtk_tooltips_set_tip (GTK_TOOLTIPS (gg->tips), tgl,
                        "Cycle through 1D plots", NULL);
  gtk_signal_connect (GTK_OBJECT (tgl), "toggled",
                      GTK_SIGNAL_FUNC (cycle_cb), (gpointer) gg);
  gtk_box_pack_start (GTK_BOX (vb), tgl, false, false, 1);


  /* value, lower, upper, step_increment, page_increment, page_size */
  /* Note that the page_size value only makes a difference for
   * scrollbar widgets, and the highest value you'll get is actually
   * (upper - page_size). */
  gg->p1d.cycle_delay_adj = (GtkAdjustment *)
    gtk_adjustment_new (-1.0 * 1000 /* cpanel->p1d.cycle_delay */,
    -5000.0, -250.0, 100.0, 1000.0, 0.0);
  gtk_signal_connect (GTK_OBJECT (gg->p1d.cycle_delay_adj), "value_changed",
                      GTK_SIGNAL_FUNC (cycle_speed_cb), gg);

  sbar = gtk_hscale_new (GTK_ADJUSTMENT (gg->p1d.cycle_delay_adj));
  gtk_tooltips_set_tip (GTK_TOOLTIPS (gg->tips), sbar,
    "Adjust cycling speed", NULL);
  scale_set_default_values (GTK_SCALE (sbar));
  gtk_box_pack_start (GTK_BOX (vb), sbar, false, false, 1);

  btn = gtk_button_new_with_label ("Change direction");
  gtk_tooltips_set_tip (GTK_TOOLTIPS (gg->tips), btn,
    "Change cycling direction", NULL);
  gtk_box_pack_start (GTK_BOX (vb), btn, false, false, 1);
  gtk_signal_connect (GTK_OBJECT (btn), "clicked",
                      GTK_SIGNAL_FUNC (chdir_cb), gg);

  gtk_widget_show_all (gg->control_panel[P1PLOT]);
}

/*--------------------------------------------------------------------*/
/*                      Control panel section                         */
/*--------------------------------------------------------------------*/

/*-- for all plot modes, for now: it excludes the changing variable --*/
void
cpanel_p1d_init (cpaneld *cpanel, ggobid *gg) {
  cpanel->p1d.nASHes = 20;
  cpanel->p1d.nbins = 200;
  cpanel->p1d.ASH_add_lines_p = false;

  /*-- cycling --*/
  cpanel->p1d.cycle_dir = FORWARD;
  cpanel->p1d.cycle_p = false;
  cpanel->p1d.cycle_delay = 1000;
}

/*-- scatterplot only; need a different routine for parcoords --*/
void
cpanel_p1d_set (cpaneld *cpanel, ggobid* gg)
/*
 * To handle the case where there are multiple scatterplots
 * which may have different p1d options and parameters selected
*/
{
  GtkWidget *pnl = gg->control_panel[P1PLOT];
  GtkWidget *w;
  GtkAdjustment *adj;

  /*-- Texturing or ASH --*/
  w = widget_find_by_name (pnl, "P1PLOT:type_option_menu");
  gtk_option_menu_set_history (GTK_OPTION_MENU (w), cpanel->p1d.type);

  /*-- ASH smoothness parameter --*/
  w = widget_find_by_name (pnl, "P1PLOT:ASH_add_lines");
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (w),
    cpanel->p1d.ASH_add_lines_p);

  /*-- ASH smoothness parameter --*/
  w = widget_find_by_name (pnl, "P1PLOT:ASH_smooth");
  adj = gtk_range_get_adjustment (GTK_RANGE (w));
  gtk_adjustment_set_value (GTK_ADJUSTMENT (adj),
    2 * (gfloat) cpanel->p1d.nASHes / (gfloat) cpanel->p1d.nbins);
/*
  gtk_adjustment_set_value (GTK_ADJUSTMENT (gg->ash.smoothness_adj),
    2 * (gfloat) cpanel->p1d.nASHes / (gfloat) cpanel->p1d.nbins);
*/

  /*-- Cycling on or off --*/
  w = widget_find_by_name (gg->control_panel[P1PLOT], "P1PLOT:cycle_toggle");
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(w), cpanel->p1d.cycle_p);

  /*-- Cycling speed --*/
  gtk_adjustment_set_value (GTK_ADJUSTMENT (gg->p1d.cycle_delay_adj),
    -1 * (gfloat) cpanel->p1d.cycle_delay);
}

