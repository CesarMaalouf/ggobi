/*-- datad.c --*/
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

#define GGOBIINTERN

#include <gtk/gtk.h>

#include "vars.h"
#include "externs.h"

#include <string.h> /* for memset() declaration */

#ifdef TESTING_ROWS_IN_PLOT_CB
void rows_in_plot_test_cb (datad *d, gint nprev, gint b, ggobid *gg,
  void *data)
{
  g_printerr ("d->nrows_in_plot = %d nprev %d\n", d->nrows_in_plot, nprev);
}
#endif


datad *
datad_new(datad *d, ggobid *gg)
{ 
  if (d == NULL) {
    d = (datad *) g_object_new(GGOBI_TYPE_DATA, NULL);
  }

  datad_instance_init(d);

  d->gg = gg;

  /*-- initialize arrays to NULL --*/
  arrayf_init_null (&d->raw);
  arrayf_init_null (&d->tform);
  arrayg_init_null (&d->world);
  arrayg_init_null (&d->jitdata);

  arrays_init_null (&d->missing);

  vectori_init_null (&d->clusterid);

  /*-- brushing and linking --*/
  vectorb_init_null (&d->edge.xed_by_brush);

  /*-- linking by categorical variable --*/
  d->linkvar_vt = NULL;

  d->rowIds = NULL;
  d->idTable = NULL;

  sphere_init (d);

  jitter_vars_init (d, gg);

#ifdef TESTING_ROWS_IN_PLOT_CB
  /*-- listen for rows_in_plot_changed events --*/
  g_signal_connect (G_OBJECT(d), "rows_in_plot_changed",
    rows_in_plot_test_cb, gg);
#endif

  gg->d = g_slist_append (gg->d, d);

  d->nclusters = 0;
  d->nearest_point = -1;

  d->missings_show_p = true;

  return (d);
}

/**
 Creates and initializes the datad object.
 */
datad*
datad_create(gint nr, gint nc, ggobid *gg)
{
  datad *d;
  d = datad_new(NULL, gg);
  d->ncols = nc;
  d->nrows = nr;

  d->nearest_point = -1;

  d->missings_show_p = true;

  vectori_init_null (&d->rows_in_plot);
  d->nrows_in_plot = d->nrows;  /*-- for now --*/

  arrayf_alloc(&d->raw, nr, nc);

  rowlabels_alloc (d);

  vartable_alloc (d);
  vartable_init (d);

  br_glyph_ids_alloc (d);
  br_glyph_ids_init (d, gg);

  br_color_ids_alloc (d, gg);
  br_color_ids_init (d, gg);

  br_hidden_alloc (d);
  br_hidden_init (d);

  return(d);
}

void
datad_free (datad *d, ggobid *gg) 
{
  arrayf_free (&d->raw, 0, 0);
  pipeline_arrays_free (d, gg);

  if (d->nmissing)
    arrays_free (&d->missing, 0, 0);


   /* rowIds and idTable are intrinsically linked !*/
  if(d->idTable) {
    g_hash_table_foreach(d->idTable, freeLevelHashEntry, d->idTable);
    g_hash_table_destroy(d->idTable); 
  }

  if(d->rowIds) 
    g_free(d->rowIds);

  g_free (d);
}

displayd *
datad_init (datad *d, ggobid *gg, gboolean cleanup)
{
  displayd *display = NULL;

  if (cleanup) {
    varpanel_clear (d, gg);
  }

  varpanel_populate (d, gg);   /*-- toggles */
  /*-- circles: build but don't show --*/
  varcircles_populate (d, gg);

  pipeline_init (d, gg);
  clusters_set (d, gg);  /*-- find the clusters for data just read in --*/

  if (cleanup || g_list_length(gg->displays) == 0) {
    display_free_all (gg);  /*-- destroy any existing displays --*/
    gg->pmode = NULL_PMODE;

    /*-- initialize the first display --*/
    if(sessionOptions->info->createInitialScatterPlot && d->ncols > 0) {
/*XXX allow this to be specified as a gtk type name on the command
      line, initialization file, etc.  and invoke the corresponding
      create() method. */
      display = scatterplot_new (false, NULL, d, gg);
        /* Need to make certain this is the only one there. */

      if (display != NULL) {
        gg->displays = g_list_append (gg->displays, (gpointer) display);
	    gg->current_splot = (splotd *)
        	g_list_nth_data (display->splots, 0);
        display->current_splot = gg->current_splot;
        display_set_current (display, gg);
        

        /*-- turn on event handling in the very first plot --*/
        /*-- ... but will it cause trouble for later plots?  ok so far
	  --*/
	/* Is imode set yet?  I hope so. */
        sp_event_handlers_toggle (gg->current_splot, on, gg->pmode, gg->imode);
      }
    }
  }

  if (gg->current_display != NULL)
    varpanel_refresh (gg->current_display, gg);

  g_signal_emit (G_OBJECT (gg), GGobiSignals[DATAD_ADDED_SIGNAL], 0, d);

  return (display);
}

/* This initializes rowIds to row numbers if ids are not provided */
void
datad_record_ids_set(datad *d, gchar **ids, gboolean duplicate)
{
  gint i;
  guint *index;
  gchar *tmp;

  d->idTable = g_hash_table_new(g_str_hash, g_str_equal);
  d->rowIds = (gchar **) g_malloc(sizeof(gchar *) * d->nrows);
  for(i = 0; i < d->nrows; i++) {
     if(ids)
       tmp = duplicate ? g_strdup(ids[i]) : ids[i];
     else {
       char buf[10];
       sprintf(buf, "%d", i+1);
       tmp = g_strdup(buf);
     }
     index = (guint *) g_malloc(sizeof(guint));
     *index = i;
     g_hash_table_insert(d->idTable, tmp, index);
     d->rowIds[i] = tmp;     
     /* do not free anything here */
  }
}

/* Add a record id */
void
datad_record_id_add (gchar *id, datad *d)
{
  gint i;
  guint *index;

  d->rowIds = (gchar **) g_realloc (d->rowIds, sizeof(gchar *) * d->nrows);
  i = d->nrows - 1;

  index = (guint *) g_malloc(sizeof(guint));
  *index = i;
  g_hash_table_insert (d->idTable, id, index);
  d->rowIds[i] = id;

/*
 * I don't really understand why I can't free this
 * when it's freed in datad_record_ids_set, but purify is
 * quite clear on this point.  -- dfs
*/
  /*g_free (index);*/
}

/*
 * Several tables use notebook widgets to separate the controls
 * corresponding to different datad's.  This is a way to figure
 * out which datad we should be operating on in that case.
*/
datad *
datad_get_from_notebook (GtkWidget *notebook, ggobid *gg) {
  datad *d = NULL;
  gint nd = g_slist_length (gg->d);

  if (nd == 1) {
    d = gg->d->data;
  } else {
    GtkNotebook *nb = GTK_NOTEBOOK (notebook);
    gint indx = gtk_notebook_get_current_page (nb);
    GtkWidget *page = gtk_notebook_get_nth_page (nb, indx);

    /*
     * Assume that each notebook page has a datad attached.
    */
    if (page) {
      d = g_object_get_data (G_OBJECT(page), "datad");
    }

    /*
     * k indexes gg->d
     * n indexes the notebook pages, so it increments only if d has variables
    */
/*
    gint k, n;
  datatyped dtype;
    dtype = (vartyped) g_object_get_data(G_OBJECT(notebook), "datatype");
    for (k = 0, n = 0; k < nd; k++) {
      d = (datad *) g_slist_nth_data (gg->d, k);

      if ((dtype == all_datatypes) ||
          (dtype == no_edgesets && d->edge.n == 0) ||
          (dtype == edgesets_only && d->edge.n > 0))
      {
        if (datad_has_variables(d)) {
          if (n == indx)
             return (d);
          n++;
        }
      }
    }
  return ((datad *) NULL);
*/
  }

  return d;
}

gint
ndatad_with_vars_get (ggobid *gg)
{
  gint nd;
  GSList *l;
  datad *d;

  /*-- silly to do this every time, perhaps, but harmless, I think --*/
  if (g_slist_length (gg->d) > 1) {
    nd = 0;
    for (l = gg->d; l; l = l->next) {
      d = (datad *) l->data;
      if (g_slist_length (d->vartable) > 0)
        nd++;
    }
  }  else nd = 1;

  return nd;
}

gchar *
datasetName (datad *d, ggobid *gg)
{
  gint which = g_slist_index (gg->d, d);
  gchar *lbl = (gchar *)NULL;

  if (d->name && d->name[0])
    lbl = g_strdup(d->name);
  else
    lbl = g_strdup_printf ("data matrix %d", which);

  return (lbl);
}

gboolean datad_has_edges (datad *d) 
{
  return (d->edge.n > 0);
}

gboolean datad_has_variables (datad *d) 
{
  return (d->ncols > 0);
}
