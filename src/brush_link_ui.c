/* brush_link.c */
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

#include <gtk/gtk.h>
#include "vars.h"
#include "externs.h"
#include "utils_ui.h"


/*********************************************************************/
/*          Create a variable notebook for brush linking rule        */
/*********************************************************************/

enum
{ LINKBYLIST_NAME, LINKBYLIST_NCOLS };

void linkby_notebook_subwindow_add (GGobiStage * d, GtkWidget * notebook,
                                    GGobiSession *);

void
varlist_append (GtkListStore * list, GGobiStage *d, gchar* name)
{
  GtkTreeIter iter;
  guint j = ggobi_stage_get_col_index_for_name(d, name);
  
  if(!GGOBI_STAGE_IS_COL_CATEGORICAL(d, j))
    return;

  gtk_list_store_append (list, &iter);
  gtk_list_store_set (list, &iter, LINKBYLIST_NAME, name, -1);
}

void
varlist_populate (GtkListStore * list, GGobiStage * d)
{
  gint j;
  GtkTreeIter first;

  gtk_list_store_append (list, &first);
  gtk_list_store_set (list, &first, LINKBYLIST_NAME, "<i>Case ID</i>", -1);

  for (j = 0; j < d->n_cols; j++) {
    varlist_append(list, d, ggobi_stage_get_col_name(d, j));
  }
}

/* In this routine, we could show two pages if appropriate, one
 * for points and one for edges. */
/* called from cpanel_brush_set */
void
linkby_current_page_set (displayd * display, GtkWidget * notebook,
                         GGobiSession * gg)
{
  GtkWidget *swin, *treeview;
  GGobiStage *d = display->d, *paged;
  gint page_num;
  cpaneld *cpanel = &display->cpanel;
  GList *children;

  if (notebook == NULL) {
    return;
  }

  /*
   * For each page of the notebook, get its child, the scrolled
   * window.  Get the datad that the scrolled window knows about,
   * and compare it with display->d
   */

  page_num = 0;
  swin = gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook), page_num);
  while (swin) {
    paged = (GGobiStage *) g_object_get_data (G_OBJECT (swin), "datad");

    gtk_widget_set_sensitive (swin, (paged == d));
    if (paged == d) {
      gtk_notebook_set_current_page (GTK_NOTEBOOK (notebook), page_num);

      children = gtk_container_get_children (GTK_CONTAINER (swin));
      treeview = children->data;
      /* Set the selected row of the selected page */
      select_tree_view_row (treeview, cpanel->br.linkby_row);
      gg->linkby_cv = (cpanel->br.linkby_row > 0);
      break;
    }
    page_num += 1;
    swin = gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook), page_num);
  }
}

void
linking_method_set_cb (GtkTreeSelection * treesel, GGobiSession * gg)
{
  GGobiStage *d =
    g_object_get_data (G_OBJECT (gtk_tree_selection_get_tree_view (treesel)),
                       "datad");
  GtkTreeModel *model;
  GtkTreeIter iter;
  GtkTreePath *path;
  gint row = -1;

  if (gtk_tree_selection_get_selected (treesel, &model, &iter)) {
    path = gtk_tree_model_get_path (model, &iter);
    row = gtk_tree_path_get_indices (path)[0];
    gtk_tree_path_free (path);
  }

  /* Before  touching the  cpanel, make  sure the  display  and cpanel
   * actually correspond to the current data.  This looks more and
   * more like a kludge -- I do need to revisit this.  -- dfs
   */
  if (gg->current_display->d == d) {
    displayd *display = gg->current_display;
    cpaneld *cpanel = &display->cpanel;
    cpanel->br.linkby_row = row;
  }

  if (row <= 0) {
    gg->linkby_cv = false;
    d->linkvar = NULL;
    return;                     /* link by case id; done */
  }
  else {
    gchar* name;
    gtk_tree_model_get (model, &iter, LINKBYLIST_NAME, &name, -1);
    gg->linkby_cv = true;
    d->linkvar = name;
  }
}

GtkListStore *
list_from_data (GGobiSession * gg, GGobiStage * data, GtkNotebook * notebook)
{
  GtkWidget *swin;
  GtkListStore *list = NULL;

  gint kd = g_slist_index (gg->d, data);
  swin = gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook), kd);

  if (swin)
    list =
      GTK_LIST_STORE (gtk_tree_view_get_model
                      (GTK_TREE_VIEW (GTK_BIN (swin)->child)));

  return (list);
}

void
linkby_notebook_varchanged_cb (GGobiSession * gg, GGobiStage * data,
                               GtkNotebook * notebook)
{
  GtkListStore *list = list_from_data (gg, data, notebook);
  gtk_list_store_clear (list);
  varlist_populate (list, data);
}

void
linkby_notebook_varadded_cb (GGobiSession * gg, gint which,
                             GGobiStage * d, GtkNotebook * notebook)
{
  GtkListStore *model = list_from_data (gg, d, notebook);
  
  if (model)
    varlist_append (model, d, ggobi_stage_get_col_name(d, which));
}

void
linkby_notebook_list_changed_cb (GGobiSession * gg, GGobiStage * d, void *notebook)
{
  linkby_notebook_varchanged_cb (gg, d, notebook);
}

CHECK_EVENT_SIGNATURE (linkby_notebook_adddata_cb, datad_added_f)
  CHECK_EVENT_SIGNATURE (linkby_notebook_varchange_cb, variable_added_f)
  CHECK_EVENT_SIGNATURE (linkby_notebook_list_changed_cb,
                       variable_list_changed_f)

static void
linkby_notebook_adddata_cb (GGobiSession * gg, GGobiStage * d, void *notebook,
                                   GtkSignalFunc func)
{
  linkby_notebook_subwindow_add (d, notebook, gg);
}

void
linkby_notebook_subwindow_add (GGobiStage * d, GtkWidget * notebook,
                               GGobiSession * gg)
{
  GtkWidget *swin, *treeview;
  GtkListStore *list;
  GGobiStage *s = ggobi_stage_find(d, GGOBI_MAIN_STAGE_DISPLAY);

  GtkSelectionMode mode = GTK_SELECTION_SINGLE;

  if (!ggobi_stage_has_vars(s))
    return;

  /* Create a scrolled window to pack the CList widget into */
  swin = gtk_scrolled_window_new (NULL, NULL);
  gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (swin),
                                       GTK_SHADOW_NONE);

  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (swin),
                                  GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);

  /* If this is not the first child of the notebook, initialize it
   * as insensitive.
   */

  if (g_list_length (gtk_container_get_children (GTK_CONTAINER (notebook))) !=
      0) {
    gtk_widget_set_sensitive (swin, false);
  }
  g_object_set_data (G_OBJECT (swin), "datad", s);  /*setdata */
/*
 * name or nickname?  Which one we'd prefer to use depends on the
 * size of the space we're working in -- maybe this will become an
 * argument.
*/
  gtk_notebook_append_page (GTK_NOTEBOOK (notebook), swin,
                            (ggobi_data_get_nickname(GGOBI_DATA(d)) != NULL) ?
                            gtk_label_new (ggobi_data_get_nickname(GGOBI_DATA(d))) : 
                            gtk_label_new (d->name));

  /* add the treeview (list) */
  list = gtk_list_store_new (LINKBYLIST_NCOLS, G_TYPE_STRING, G_TYPE_POINTER);
  varlist_populate (list, s);

  treeview = gtk_tree_view_new_with_model (GTK_TREE_MODEL (list));
  populate_tree_view (treeview, NULL, 1, false, mode,
                      G_CALLBACK (linking_method_set_cb), gg);

  g_object_set_data (G_OBJECT (treeview), "datad", s);
  //g_object_set_data (G_OBJECT (clist), "notebook", notebook);

  gtk_container_add (GTK_CONTAINER (swin), treeview);
  gtk_widget_show_all (swin);

  select_tree_view_row (treeview, 0);
}

GtkWidget *
create_linkby_notebook (GtkWidget * box, GGobiSession * gg)
{
  GtkWidget *notebook;
  GSList *l;
  GGobiStage *d;

  GtkSelectionMode mode = GTK_SELECTION_SINGLE;
  GGobiVariableType vartype = GGOBI_VARIABLE_CATEGORICAL;
  //datatyped dtype = all_datatypes;

  /* Create a notebook, set the position of the tabs */
  notebook = gtk_notebook_new ();
  gtk_notebook_set_tab_pos (GTK_NOTEBOOK (notebook), GTK_POS_TOP);
  //gtk_notebook_set_show_tabs (GTK_NOTEBOOK (notebook), nd > 1);
  gtk_notebook_set_show_tabs (GTK_NOTEBOOK (notebook), false);
  gtk_box_pack_start (GTK_BOX (box), notebook, true, true, 2);
  g_object_set_data (G_OBJECT (notebook), "SELECTION", (gpointer) mode);
  g_object_set_data (G_OBJECT (notebook), "vartype", (gpointer) vartype);
  //g_object_set_data (G_OBJECT (notebook), "datatype", (gpointer) dtype);

  for (l = gg->d; l; l = l->next) {
    d = (GGobiStage *) l->data;
    if (ggobi_stage_has_vars(d)) {
      linkby_notebook_subwindow_add (d, notebook, gg);
    }
  }

  /*-- listen for variable_added and _list_changed events on main_window --*/
  /*--   ... list_changed would be enough but it's only called on delete --*/
  g_signal_connect (G_OBJECT (gg),
                    "variable_added",
                    G_CALLBACK (linkby_notebook_varadded_cb),
                    GTK_OBJECT (notebook));
  g_signal_connect (G_OBJECT (gg),
                    "variable_list_changed",
                    G_CALLBACK (linkby_notebook_varchanged_cb),
                    GTK_OBJECT (notebook));

  /*-- listen for datad_added events on main_window --*/
  g_signal_connect (G_OBJECT (gg),
                    "datad_added",
                    G_CALLBACK (linkby_notebook_adddata_cb),
                    GTK_OBJECT (notebook));

  return notebook;
}
