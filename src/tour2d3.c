/* tour2d3.c */
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

#include <gtk/gtk.h>
#ifdef USE_STRINGS_H
#include <strings.h>
#endif

#include <math.h>
#include <unistd.h>

#include "vars.h"
#include "externs.h"

#define T2D3ON  true
#define T2D3OFF false

void
display_tour2d3_init_null (displayd *dsp, GGobiSession *gg)
{
  arrayd_init_null(&dsp->t2d3.Fa);
  arrayd_init_null(&dsp->t2d3.Fz);
  arrayd_init_null(&dsp->t2d3.F);

  arrayd_init_null(&dsp->t2d3.Ga);
  arrayd_init_null(&dsp->t2d3.Gz);
  arrayd_init_null(&dsp->t2d3.G);

  arrayd_init_null(&dsp->t2d3.Va);
  arrayd_init_null(&dsp->t2d3.Vz);

  arrayd_init_null(&dsp->t2d3.tv);

  vectori_init_null(&dsp->t2d3.subset_vars);
  vectorb_init_null(&dsp->t2d3.subset_vars_p);
  vectori_init_null(&dsp->t2d3.active_vars);
  vectorb_init_null(&dsp->t2d3.active_vars_p);

  vectord_init_null(&dsp->t2d3.lambda);
  vectord_init_null(&dsp->t2d3.tau);
  vectord_init_null(&dsp->t2d3.tinc);

  /* manipulation variables */
  arrayd_init_null(&dsp->t2d3_Rmat1);
  arrayd_init_null(&dsp->t2d3_Rmat2);
  arrayd_init_null(&dsp->t2d3_mvar_3dbasis);
  arrayd_init_null(&dsp->t2d3_manbasis);
}

void
alloc_tour2d3 (displayd *dsp, GGobiSession *gg)
{
  GGobiStage *d = dsp->d;
  gint nc = d->n_cols;

  /* first index is the projection dimensions, second dimension is ncols */
  arrayd_alloc(&dsp->t2d3.Fa, 2, nc);
  arrayd_alloc(&dsp->t2d3.Fz, 2, nc);
  arrayd_alloc(&dsp->t2d3.F, 2, nc);

  arrayd_alloc(&dsp->t2d3.Ga, 2, nc);
  arrayd_alloc(&dsp->t2d3.Gz, 2, nc);
  arrayd_alloc(&dsp->t2d3.G, 2, nc);

  arrayd_alloc(&dsp->t2d3.Va, 2, nc);
  arrayd_alloc(&dsp->t2d3.Vz, 2, nc);

  arrayd_alloc(&dsp->t2d3.tv, 2, nc);

  vectori_alloc(&dsp->t2d3.subset_vars, nc);
  vectorb_alloc_zero(&dsp->t2d3.subset_vars_p, nc);
  vectori_alloc(&dsp->t2d3.active_vars, nc);
  vectorb_alloc_zero(&dsp->t2d3.active_vars_p, nc);

  vectord_alloc(&dsp->t2d3.lambda, nc);
  vectord_alloc_zero(&dsp->t2d3.tau, nc);
  vectord_alloc(&dsp->t2d3.tinc, nc);

  /* manipulation variables */
  arrayd_alloc(&dsp->t2d3_Rmat1, 3, 3);
  arrayd_alloc(&dsp->t2d3_Rmat2, 3, 3);
  arrayd_alloc(&dsp->t2d3_mvar_3dbasis, 3, 3);
  arrayd_alloc(&dsp->t2d3_manbasis, 3, nc);
}

/*-- eliminate the nc columns contained in *cols --*/
void
tour2d3_realloc_down (GSList *cols, GGobiStage *d, GGobiSession *gg)
{
  displayd *dsp;
  GList *l;
  for (l=gg->displays; l; l=l->next) {
    dsp = (displayd *) l->data;
    if (dsp->d == d) {
      arrayd_delete_cols (&dsp->t2d3.Fa, cols);
      arrayd_delete_cols (&dsp->t2d3.Fz, cols);
      arrayd_delete_cols (&dsp->t2d3.F, cols);
      arrayd_delete_cols (&dsp->t2d3.Ga, cols);
      arrayd_delete_cols (&dsp->t2d3.Gz, cols);
      arrayd_delete_cols (&dsp->t2d3.G, cols);
      arrayd_delete_cols (&dsp->t2d3.Va, cols);
      arrayd_delete_cols (&dsp->t2d3.Vz, cols);
      arrayd_delete_cols (&dsp->t2d3.tv, cols);

      vectori_delete_els (&dsp->t2d3.subset_vars, cols);
      vectorb_delete_els (&dsp->t2d3.subset_vars_p, cols);
      vectori_delete_els (&dsp->t2d3.active_vars, cols);
      vectorb_delete_els (&dsp->t2d3.active_vars_p, cols);

      vectord_delete_els (&dsp->t2d3.lambda, cols);
      vectord_delete_els (&dsp->t2d3.tau, cols);
      vectord_delete_els (&dsp->t2d3.tinc, cols);

      arrayd_delete_cols (&dsp->t2d3_manbasis, cols);
    }
  }
}

void
free_tour2d3(displayd *dsp)
{
  vectori_free(&dsp->t2d3.subset_vars);
  vectorb_free(&dsp->t2d3.subset_vars_p);
  vectori_free(&dsp->t2d3.active_vars);
  vectorb_free(&dsp->t2d3.active_vars_p);

  vectord_free(&dsp->t2d3.lambda);
  vectord_free(&dsp->t2d3.tau);
  vectord_free(&dsp->t2d3.tinc);

  arrayd_free (&dsp->t2d3.Fa); 
  arrayd_free (&dsp->t2d3.Fz); 
  arrayd_free (&dsp->t2d3.F); 

  arrayd_free (&dsp->t2d3.Ga); 
  arrayd_free (&dsp->t2d3.Gz); 
  arrayd_free (&dsp->t2d3.G); 

  arrayd_free (&dsp->t2d3.Va); 
  arrayd_free (&dsp->t2d3.Vz); 
  arrayd_free (&dsp->t2d3.tv); 

  arrayd_free (&dsp->t2d3_Rmat1); 
  arrayd_free (&dsp->t2d3_Rmat2); 
  arrayd_free (&dsp->t2d3_mvar_3dbasis); 
  arrayd_free (&dsp->t2d3_manbasis); 
}

void 
display_tour2d3_init (displayd *dsp, GGobiSession *gg) {
  gint i, j;
  GGobiStage *d = dsp->d;
  cpaneld *cpanel = &dsp->cpanel;
  gint nc = d->n_cols;

  if (nc < MIN_NVARS_FOR_TOUR2D3)
    return;

  alloc_tour2d3(dsp, gg);
 
/*
 * subset_vars.els[0] = VARSEL_X
 * subset_vars.els[1] = VARSEL_Y
 * subset_vars.els[2] = VARSEL_Z
*/
  dsp->t2d3.nsubset = dsp->t2d3.nactive = 3;
  for (j=0; j<nc; j++) {
    dsp->t2d3.subset_vars.els[j] = dsp->t2d3.active_vars.els[j] = 0;
    dsp->t2d3.subset_vars_p.els[j] = dsp->t2d3.active_vars_p.els[j] = false;
  }
  for (j=0; j<3; j++) {
    dsp->t2d3.subset_vars.els[j] = dsp->t2d3.active_vars.els[j] = j;
    dsp->t2d3.subset_vars_p.els[j] = dsp->t2d3.active_vars_p.els[j] = true;
  }

  /* declare starting base as first 2 chosen variables */
  arrayd_zero (&dsp->t2d3.Fa);
  arrayd_zero (&dsp->t2d3.Fz);
  arrayd_zero (&dsp->t2d3.F);
  arrayd_zero (&dsp->t2d3.Ga);
  arrayd_zero (&dsp->t2d3.Gz);

  for (i=0; i<2; i++) {
    dsp->t2d3.Fz.vals[i][dsp->t2d3.active_vars.els[i]] =
      dsp->t2d3.Fa.vals[i][dsp->t2d3.active_vars.els[i]] = 
      dsp->t2d3.F.vals[i][dsp->t2d3.active_vars.els[i]] =
      dsp->t2d3.Ga.vals[i][dsp->t2d3.active_vars.els[i]] = 
      dsp->t2d3.Gz.vals[i][dsp->t2d3.active_vars.els[i]] = 1.0;
  }

  dsp->t2d3.dist_az = 0.0;
  dsp->t2d3.delta = cpanel->t2d3.step*M_PI_2/10.0;
  dsp->t2d3.tang = 0.0;

  dsp->t2d3.idled = 0;
  dsp->t2d3.get_new_target = true;

  /* manip */
  dsp->t2d3_manip_var = 0;

  dsp->t2d.target_selection_method = 0;
}

void tour2d3_speed_set(gdouble slidepos, GGobiSession *gg) {
  displayd *dsp = gg->current_display; 
  cpaneld *cpanel = &dsp->cpanel;

  // Tour timeout testing -- dfs
#if 0
  {
    if (fabs(cpanel->t2d3.slidepos - slidepos) > .001) {
      // Remove and re-add the timeout in order to change delay
      tour2d3_func(off, dsp, gg);
      tour2d3_func(on, dsp, gg);
    }
  }
#endif
  cpanel->t2d3.slidepos = slidepos;
  speed_set(slidepos, &cpanel->t2d3.step, &dsp->t2d3.delta);
}

void tour2d3_pause (cpaneld *cpanel, gboolean state, GGobiSession *gg) {
  displayd *dsp = gg->current_display;
  GGobiStage *d = dsp->d;
  cpanel->t2d3.paused = state;

  tour2d3_func (!cpanel->t2d3.paused, gg->current_display, gg);

  if (cpanel->t2d3.paused) {
    /*-- whenever motion stops, we need a FULL redraw --*/
    display_tailpipe (gg->current_display, FULL, gg);
    varcircles_refresh (d, gg);
  }
}

/*-- add/remove jvar to/from the subset of variables that <may> be active --*/
gboolean
tour2d3_subset_var_set (gint jvar, gint *jprev, gint toggle, GGobiStage *d,
  displayd *dsp, GGobiSession *gg)
{
  gboolean in_subset = dsp->t2d3.subset_vars_p.els[jvar];
  gint j, k;
  gboolean changed = false;
  gint xyz;

  *jprev = dsp->t2d3.subset_vars.els[toggle];

  /*-- require exactly 3 variables in the subset --*/
  if (in_subset) {               /*-- handle a swap --*/

    if (dsp->t2d3.subset_vars.els[toggle] == jvar)
/**/  return false;

    switch (toggle) {
      case VARSEL_X:
        xyz = (jvar == dsp->t2d3.subset_vars.els[VARSEL_Y]) ?
          VARSEL_Y : VARSEL_Z;
      break;
      case VARSEL_Y:
        xyz = (jvar == dsp->t2d3.subset_vars.els[VARSEL_X]) ?
          VARSEL_X : VARSEL_Z;
      break;
      case VARSEL_Z:
        xyz = (jvar == dsp->t2d3.subset_vars.els[VARSEL_X]) ?
          VARSEL_X : VARSEL_Y;
      break;
      default:
/**/    return false;
      break;
    }

    dsp->t2d3.subset_vars.els[xyz] = dsp->t2d3.subset_vars.els[toggle];
    dsp->t2d3.subset_vars.els[toggle] = jvar;
    changed = true;
  } else {
    dsp->t2d3.subset_vars.els[toggle] = jvar;
    changed = true;
  }

  /*-- reset subset_vars_p and active_vars based on subset_vars: 
   unlike other tour modes --*/
  if (changed) {
    dsp->t2d3_manipvar_inc = false;
    for (j=0; j<d->n_cols; j++) {
      dsp->t2d3.subset_vars_p.els[j] = false;
    }
    for (j=0; j<3; j++) {
      k = dsp->t2d3.subset_vars.els[j];
      dsp->t2d3.subset_vars_p.els[k] = true;
      if (k == dsp->t2d3_manip_var)
        dsp->t2d3_manipvar_inc = true;
    }
    /*-- Manip var needs to be one of the active vars --*/
    if (!dsp->t2d3_manipvar_inc) 
      dsp->t2d3_manip_var = dsp->t2d3.subset_vars.els[0];

    zero_tau(dsp->t2d3.tau, 2);
    dsp->t2d3.get_new_target = true;
  }

  return changed;
}

/*-- add or remove jvar from the set of active variables --*/
void 
tour2d3_active_vars_swap (gint jvar_out, gint jvar_in, GGobiStage *d,
  displayd *dsp, GGobiSession *gg)
{
  gint k;
  gint a, b;
  gboolean both_in_subset = dsp->t2d3.subset_vars_p.els[jvar_out] &&
                            dsp->t2d3.subset_vars_p.els[jvar_in];

  if (both_in_subset) {  /* swap their positions in active_vars */
    a = b = -1;
    for (k=0; k<dsp->t2d3.nactive; k++) {
      if (dsp->t2d3.active_vars.els[k] == jvar_in)
        a = k;
      else if (dsp->t2d3.active_vars.els[k] == jvar_out)
        b = k;
    }
    if (a != -1 && b != -1) {
      dsp->t2d3.active_vars.els[a] = jvar_out;
      dsp->t2d3.active_vars.els[b] = jvar_in;
    }

  } else {
    dsp->t2d3.active_vars_p.els[jvar_out] = false;
    dsp->t2d3.active_vars_p.els[jvar_in] = true;

    for (k=0; k<dsp->t2d3.nactive; k++)
      if (dsp->t2d3.active_vars.els[k] == jvar_out)
        dsp->t2d3.active_vars.els[k] = jvar_in;
  }

  gt_basis(dsp->t2d3.Fa, dsp->t2d3.nactive, dsp->t2d3.active_vars, 
    d->n_cols, (gint) 2);
  arrayd_copy(&dsp->t2d3.Fa, &dsp->t2d3.F);

  zero_tau(dsp->t2d3.tau, 2);
  dsp->t2d3.get_new_target = true;
}

static void
tour2d3_manip_var_set (gint j, GGobiSession *gg)
{
  displayd *dsp = gg->current_display;

  dsp->t2d3_manip_var = j;    
}

gboolean
tour2d3_varsel (GtkWidget *w, gint jvar, gint toggle, gint mouse, GGobiStage *d,
  GGobiSession *gg)
{
  displayd *dsp = gg->current_display;
  gboolean changed = true;
  gint jprev;

  if (GTK_IS_TOGGLE_BUTTON(w)) {
    /* add/remove jvar to/from the subset of variables that <may> be active */

    changed = tour2d3_subset_var_set(jvar, &jprev, toggle, d, dsp, gg);
    if (changed) {
      varcircles_visibility_set (dsp, gg);
      tour2d3_active_vars_swap (jprev, jvar, d, dsp, gg);
      display_tailpipe (gg->current_display, FULL, gg);
      varcircles_refresh (d, gg);
    }
  } else if (GTK_IS_BUTTON(w)) {  /*-- it's the label --*/

    /*-- 'mouse' is the mouse button; translate it to one of the toggles --*/
    changed = tour2d3_subset_var_set(jvar, &jprev, mouse-1, d, dsp, gg);
    if (changed) {
      varcircles_visibility_set (dsp, gg);
      tour2d3_active_vars_swap (jprev, jvar, d, dsp, gg);
      display_tailpipe (gg->current_display, FULL, gg);
      varcircles_refresh (d, gg);
    }

  } else if (GTK_IS_DRAWING_AREA(w)) {
    
    if (d->vcirc_ui.jcursor == GDK_HAND2) {
      tour2d3_manip_var_set (jvar, gg);
      varcircles_cursor_set_default (d);
    }
  }

  return changed;
}

void
tour2d3_projdata(splotd *sp, gdouble **world_data, GGobiStage *d, GGobiSession *gg)
{
  gint j, m;
  displayd *dsp = (displayd *) sp->displayptr;
  gdouble tmpf, maxx, maxy;

  if (sp->tour2d3.initmax) {
    sp->tour2d3.maxscreen = 1;
    sp->tour2d3.initmax = false;
  }

  tmpf = 1/sp->tour2d3.maxscreen;
  maxx = sp->tour2d3.maxscreen;
  maxy = sp->tour2d3.maxscreen;
  for (m=0; m<d->n_rows; m++)
  {
    sp->planar[m].x = 0;
    sp->planar[m].y = 0;
    for (j=0; j<d->n_cols; j++)
    {
      sp->planar[m].x += (gdouble)(dsp->t2d3.F.vals[0][j]*world_data[m][j]);
      sp->planar[m].y += (gdouble)(dsp->t2d3.F.vals[1][j]*world_data[m][j]);
    }
    sp->planar[m].x *= tmpf;
    sp->planar[m].y *= tmpf;
    if (fabs(sp->planar[m].x) > maxx)
      maxx = fabs(sp->planar[m].x);
    if (fabs(sp->planar[m].y) > maxy)
      maxy = fabs(sp->planar[m].y);
  }

  if ((maxx > 1) || (maxy > 1)) {
    sp->tour2d3.maxscreen = (maxx > maxy) ? maxx : maxy;
    tmpf = 1/tmpf;
  }
}

void tour2d3_scramble(GGobiSession *gg)
{
  displayd *dsp = gg->current_display;
  GGobiStage *d = dsp->d;

  arrayd_zero (&dsp->t2d3.Fa);
  arrayd_zero (&dsp->t2d3.Fz);
  arrayd_zero (&dsp->t2d3.F);
  arrayd_zero (&dsp->t2d3.Ga);
  arrayd_zero (&dsp->t2d3.Gz);

  gt_basis(dsp->t2d3.Fa, dsp->t2d3.nactive, dsp->t2d3.active_vars, 
    d->n_cols, (gint) 2);
  arrayd_copy(&dsp->t2d3.Fa, &dsp->t2d3.F);

  dsp->t2d3.tau.els[0] = 0.0;
  dsp->t2d3.tau.els[1] = 0.0;
  dsp->t2d3.get_new_target = true;

  display_tailpipe (dsp, FULL, gg);

  varcircles_refresh (d, gg);
}

void
tour2d3_run(displayd *dsp, GGobiSession *gg)
{
  GGobiStage *d = dsp->d;
  gint i, nv;
  gint k;
  gboolean chosen;
  gdouble eps = .01;
  gint pathprob = 0;

  if (!dsp->t2d3.get_new_target && 
      !reached_target(dsp->t2d3.tang, dsp->t2d3.dist_az,
       dsp->t2d3.target_selection_method, &dsp->t2d3.ppval, &dsp->t2d3.oppval))
  {
    increment_tour(dsp->t2d3.tinc, dsp->t2d3.tau, dsp->t2d3.dist_az, 
      .01, &dsp->t2d3.tang, (gint) 2);
    /*      dsp->t2d3.delta, &dsp->t2d3.tang, (gint) 2); */
    tour_reproject(dsp->t2d3.tinc, dsp->t2d3.G, dsp->t2d3.Ga, dsp->t2d3.Gz, 
      dsp->t2d3.F, dsp->t2d3.Va, d->n_cols, (gint) 2);

  }
  else { /* do final clean-up and get new target */
    if (dsp->t2d3.tau.els[0] > 0.0 || dsp->t2d3.tau.els[1] > 0.0) {
      do_last_increment(dsp->t2d3.tinc, dsp->t2d3.tau, 
        dsp->t2d3.dist_az, (gint) 2);
      tour_reproject(dsp->t2d3.tinc, dsp->t2d3.G, dsp->t2d3.Ga,
        dsp->t2d3.Gz,
        dsp->t2d3.F, dsp->t2d3.Va, d->n_cols, (gint) 2);
    }

    nv = 0;
    for (i=0; i<d->n_cols; i++) {
      chosen = false;
      for (k=0; k<dsp->t2d3.nactive; k++) {
        if (dsp->t2d3.active_vars.els[k] == i) {
          chosen = true;
          break;
        }
      }
      if (!chosen) {
        if (fabs(dsp->t2d3.F.vals[0][i]) < eps && 
          fabs(dsp->t2d3.F.vals[1][i]) < eps)
          dsp->t2d3.F.vals[0][i] = dsp->t2d3.F.vals[1][i] = 0.0;
        if (fabs(dsp->t2d3.F.vals[0][i]) > eps || 
          fabs(dsp->t2d3.F.vals[1][i]) > eps)
        {
          nv++;
        }
      }
    }
    arrayd_copy(&dsp->t2d3.F, &dsp->t2d3.Fa);
    if (nv == 0 && dsp->t2d3.nactive <= 2) /* only generate new dir if num of
                                           active/used variables is > 2 -
                                           this code allows for motion to
                                           continue while a variable is 
                                           fading out. */
      dsp->t2d3.get_new_target = true;
    else {
      gt_basis(dsp->t2d3.Fz, dsp->t2d3.nactive, dsp->t2d3.active_vars, 
        d->n_cols, (gint) 2);

      pathprob = tour_path(dsp->t2d3.Fa, dsp->t2d3.Fz, dsp->t2d3.F, d->n_cols, 
        (gint) 2, dsp->t2d3.Ga,
        dsp->t2d3.Gz, dsp->t2d3.G, dsp->t2d3.lambda, dsp->t2d3.tv, dsp->t2d3.Va,
        dsp->t2d3.Vz, dsp->t2d3.tau, dsp->t2d3.tinc, 
        &dsp->t2d3.dist_az, &dsp->t2d3.tang);
      if (pathprob == 0) 
        dsp->t2d3.get_new_target = false;
      else if (pathprob == 1) { /* problems with Fa so need to force a jump */
        tour2d3_scramble(gg);
        pathprob = tour_path(dsp->t2d3.Fa, dsp->t2d3.Fz, dsp->t2d3.F, d->n_cols, 
          (gint) 2, dsp->t2d3.Ga,
          dsp->t2d3.Gz, dsp->t2d3.G, dsp->t2d3.lambda, dsp->t2d3.tv,
          dsp->t2d3.Va,
          dsp->t2d3.Vz, dsp->t2d3.tau, dsp->t2d3.tinc, 
          &dsp->t2d3.dist_az, &dsp->t2d3.tang);
      }
      else if (pathprob == 2 || pathprob == 3) { /* problems with Fz,
                                    so will force a new choice of Fz */
        dsp->t2d3.get_new_target = true;
      }
    }
  }
  
  display_tailpipe (dsp, FULL_1PIXMAP, gg);
  varcircles_refresh (d, gg);
}

void
tour2d3_do_step(displayd *dsp, GGobiSession *gg)
{
  tour2d3_run(dsp, gg);
}

gint
tour2d3_idle_func (displayd *dsp)
{
  GGobiSession *gg = GGobiFromDisplay (dsp);
  cpaneld *cpanel = &dsp->cpanel;
  gboolean doit = !cpanel->t2d3.paused;

  if (doit) {
    tour2d3_run (dsp, gg);
    gdk_flush ();
  }

  return (doit);
}

void tour2d3_func (gboolean state, displayd *dsp, GGobiSession *gg)
{
  splotd *sp = (splotd *) g_list_nth_data (dsp->splots, 0);

  if (state) {
    if (dsp->t2d3.idled == 0) {
      dsp->t2d3.idled = g_idle_add_full (G_PRIORITY_LOW,
                                   (GtkFunction) tour2d3_idle_func, dsp, NULL);
    }
    gg->tour2d3.idled = 1; // Is this ever used?
  } else {
    if (dsp->t2d3.idled != 0) {
      g_source_remove (dsp->t2d3.idled);
      dsp->t2d3.idled = 0;
    }
    gg->tour2d3.idled = 0;  // Is this ever used?
  }

  splot_connect_expose_handler (dsp->t2d3.idled, sp);
}

#if 0
// Tour timeout testing -- dfs
void tour2d3_func (gboolean state, displayd *dsp, ggobid *gg)
{
  if (state) {
    if (dsp->t2d3.timeout_id == 0) {
      cpaneld *cpanel = &dsp->cpanel;
      dsp->t2d3.timeout_id = g_timeout_add (1*(100-cpanel->t2d3.slidepos),
				       (GSourceFunc) tour2d3_idle_func,
      				       (gpointer) dsp);
    }
  } else {
    if (dsp->t2d3.timeout_id != 0) {
      g_source_remove (dsp->t2d3.timeout_id);
      dsp->t2d3.timeout_id = 0;
    }
  }
}
#endif

void tour2d3_reinit(GGobiSession *gg)
{
  gint i;
  displayd *dsp = gg->current_display;
  GGobiStage *d = dsp->d;
  splotd *sp = gg->current_splot;

  arrayd_zero (&dsp->t2d3.Fa);
  arrayd_zero (&dsp->t2d3.Fz);
  arrayd_zero (&dsp->t2d3.F);
  arrayd_zero (&dsp->t2d3.Ga);
  arrayd_zero (&dsp->t2d3.Gz);

  for (i=0; i<2; i++) {
    dsp->t2d3.Fz.vals[i][dsp->t2d3.active_vars.els[i]] =
      dsp->t2d3.Fa.vals[i][dsp->t2d3.active_vars.els[i]] = 
      dsp->t2d3.F.vals[i][dsp->t2d3.active_vars.els[i]] =
      dsp->t2d3.Ga.vals[i][dsp->t2d3.active_vars.els[i]] = 
      dsp->t2d3.Gz.vals[i][dsp->t2d3.active_vars.els[i]] = 1.0;
  }

  dsp->t2d3.get_new_target = true;
  sp->tour2d3.initmax = true;

  display_tailpipe (dsp, FULL, gg);
  varcircles_refresh (d, gg);
}

/* Variable manipulation */
void
tour2d3_manip_init(gint p1, gint p2, splotd *sp) 
{
  displayd *dsp = (displayd *) sp->displayptr;
  GGobiStage *d = dsp->d;
  cpaneld *cpanel = &dsp->cpanel;
  GGobiSession *gg = GGobiFromSPlot(sp);
  gint j, k;
  gint n1vars = dsp->t2d3.nactive;
  gdouble tol = 0.05; 
  gdouble dtmp1;

  /* need to turn off tour */
  if (!cpanel->t2d3.paused)
    tour2d3_func(T2D3OFF, gg->current_display, gg);

  /* If de-selected variables are still fading out of the tour
     we will need to take them out before starting manipulation */
  for (j=0; j<d->n_cols; j++)
    if (dsp->t2d3.active_vars_p.els[j] == false) {
       if (dsp->t2d3.F.vals[0][j] > 0.0) 
         dsp->t2d3.F.vals[0][j] = 0.0;
       if (dsp->t2d3.F.vals[1][j] > 0.0)
         dsp->t2d3.F.vals[1][j] = 0.0;
    }
  norm(dsp->t2d3.F.vals[0],d->n_cols);
  norm(dsp->t2d3.F.vals[1],d->n_cols);
  if (!gram_schmidt(dsp->t2d3.F.vals[0], dsp->t2d3.F.vals[1],
    d->n_cols))
#ifdef EXCEPTION_HANDLING
    g_printerr("");/*t2d3.F[0] equivalent to t2d3.F[1]\n");*/
#else
      ;
#endif
  
  dsp->t2d3_manipvar_inc = false;
  dsp->t2d3_pos1 = dsp->t2d3_pos1_old = p1;
  dsp->t2d3_pos2 = dsp->t2d3_pos2_old = p2;
  /* check if manip var is one of existing vars */
  /* n1vars, n2vars is the number of variables, excluding the
     manip var in hor and vert directions */
  for (j=0; j<dsp->t2d3.nactive; j++)
    if (dsp->t2d3.active_vars.els[j] == dsp->t2d3_manip_var) {
      dsp->t2d3_manipvar_inc = true;
      n1vars--;
    }
  /* the manip var has to be one of the active variables in this
     mode, so if it is not, then set it to be the first active variable */
  if (!dsp->t2d3_manipvar_inc) {
    dsp->t2d3_manip_var = dsp->t2d3.active_vars.els[0];
    dsp->t2d3_manipvar_inc = true;
    n1vars--;
  }

  /* here need to check if the manip var is wholly contained in u, and
     if so do some check */

  if (n1vars > 1)
  {
    /* make manip basis, from existing projection */
    /* 0,1 will be the remainder of the projection, and
       2 will be the indicator vector for the manip var */
    for (j=0; j<d->n_cols; j++) 
    {
      dsp->t2d3_manbasis.vals[0][j] = dsp->t2d3.F.vals[0][j];
      dsp->t2d3_manbasis.vals[1][j] = dsp->t2d3.F.vals[1][j];
      dsp->t2d3_manbasis.vals[2][j] = 0.;
    }
    dsp->t2d3_manbasis.vals[2][dsp->t2d3_manip_var] = 1.;

    for (j=0; j<3; j++) {
      for (k=0; k<3; k++)
        dsp->t2d3_mvar_3dbasis.vals[j][k] = 0.;
      dsp->t2d3_mvar_3dbasis.vals[j][j] = 1.;
    }

    norm(dsp->t2d3_manbasis.vals[0], d->n_cols); /* this is just in case */
    norm(dsp->t2d3_manbasis.vals[1], d->n_cols); /* it seems to work ok */
    norm(dsp->t2d3_manbasis.vals[2], d->n_cols); /* without normalizing here */

    /* Check if column 3 (2) of manbasis is effectively equal to 
       column 1 (0) or 2(1). If they are then we'll have to randomly
       generate a new column 3. If not then we orthonormalize column 3
       on the other two. */
    while (!gram_schmidt(dsp->t2d3_manbasis.vals[0], 
           dsp->t2d3_manbasis.vals[2], d->n_cols))
    {
      gt_basis(dsp->t2d3.tv, dsp->t2d3.nactive, dsp->t2d3.active_vars, 
        d->n_cols, (gint) 1);
      for (j=0; j<d->n_cols; j++) 
        dsp->t2d3_manbasis.vals[2][j] = dsp->t2d3.tv.vals[0][j];
    }
    while (!gram_schmidt(dsp->t2d3_manbasis.vals[1], 
           dsp->t2d3_manbasis.vals[2], d->n_cols))
    {
      gt_basis(dsp->t2d3.tv, dsp->t2d3.nactive, dsp->t2d3.active_vars, 
        d->n_cols, (gint) 1);
      for (j=0; j<d->n_cols; j++) 
        dsp->t2d3_manbasis.vals[2][j] = dsp->t2d3.tv.vals[0][j];
    }
    while (!gram_schmidt(dsp->t2d3_manbasis.vals[0],
           dsp->t2d3_manbasis.vals[1], d->n_cols))
    {
      gt_basis(dsp->t2d3.tv, dsp->t2d3.nactive, dsp->t2d3.active_vars, 
        d->n_cols, (gint) 1);
      for (j=0; j<d->n_cols; j++) 
        dsp->t2d3_manbasis.vals[1][j] = dsp->t2d3.tv.vals[0][j];
    }

    /* This is innocuous, if the vectors are orthnormal nothing gets changed.
       But it protects against the case when vectors 0,1 were not
       orthonormal and a new vector 1 was generated, it checks the o.n.
       of all 3 vectors again. */
    gram_schmidt(dsp->t2d3_manbasis.vals[0],  dsp->t2d3_manbasis.vals[1],
      d->n_cols);
    gram_schmidt(dsp->t2d3_manbasis.vals[0],  dsp->t2d3_manbasis.vals[2],
      d->n_cols);
    gram_schmidt(dsp->t2d3_manbasis.vals[1],  dsp->t2d3_manbasis.vals[2],
      d->n_cols);

    /*    ftmp = 0.0;
    while (ftmp < tol) {
    if ((fabs(inner_prod(dsp->t2d3_manbasis.vals[0],dsp->t2d3_manbasis.vals[2],
       d->n_cols))>1.0-tol) || 
       (fabs(inner_prod(dsp->t2d3_manbasis.vals[1],
       dsp->t2d3_manbasis.vals[2],d->n_cols))>1.0-tol))
    {
      gt_basis(dsp->t2d3.tv, dsp->t2d3.nactive, dsp->t2d3.active_vars, 
        d->n_cols, (gint) 1);
      for (j=0; j<d->n_cols; j++) 
        dsp->t2d3_manbasis.vals[2][j] = dsp->t2d3.tv.vals[0][j];
      g_printerr("0 manbasis2: ");
        for (i=0; i<3; i++)
          g_printerr("%f ",dsp->t2d3_manbasis.vals[2][i]);
      g_printerr("\n");
      if (!gram_schmidt(dsp->t2d3_manbasis.vals[0],  dsp->t2d3_manbasis.vals[2],
        d->n_cols)) 
        g_printerr("t2d3_manbasis[0] equivalent to t2d3_manbasis[2]\n");
      if (!gram_schmidt(dsp->t2d3_manbasis.vals[1],  dsp->t2d3_manbasis.vals[2],
        d->n_cols))
        g_printerr("t2d3_manbasis[1] equivalent to t2d3_manbasis[2]\n");

        g_printerr("1 manbasis0: ");
        for (i=0; i<3; i++)
          g_printerr("%f ",dsp->t2d3_manbasis.vals[0][i]);
        g_printerr("\n");
        g_printerr("1 manbasis1: ");
        for (i=0; i<3; i++)
          g_printerr("%f ",dsp->t2d3_manbasis.vals[1][i]);
        g_printerr("\n");
        g_printerr("1 manbasis2: ");
        for (i=0; i<3; i++)
          g_printerr("%f ",dsp->t2d3_manbasis.vals[2][i]);
        g_printerr("\n");
      ftmp = calc_norm (dsp->t2d3_manbasis.vals[2], d->n_cols);
    }
    else if (fabs(inner_prod(dsp->t2d3_manbasis.vals[0],
      dsp->t2d3_manbasis.vals[1],d->n_cols))>1.0-tol) 
    {
      printf("1 = 0\n");
      gt_basis(dsp->t2d3.tv, dsp->t2d3.nactive, dsp->t2d3.active_vars, 
        d->n_cols, (gint) 1);
      for (j=0; j<d->n_cols; j++) 
        dsp->t2d3_manbasis.vals[1][j] = dsp->t2d3.tv.vals[0][j];
      if (!gram_schmidt(dsp->t2d3_manbasis.vals[0],  dsp->t2d3_manbasis.vals[1],
		   d->n_cols))
        g_printerr("t2d3_manbasis[0] equivalent to t2d3_manbasis[1]\n"); * this might not be necessary *
      if (!gram_schmidt(dsp->t2d3_manbasis.vals[0],  dsp->t2d3_manbasis.vals[2],
        d->n_cols))
        g_printerr("t2d3_manbasis[0] equivalent to t2d3_manbasis[2]\n");
      if (!gram_schmidt(dsp->t2d3_manbasis.vals[1],  dsp->t2d3_manbasis.vals[2],
        d->n_cols))
        g_printerr("t2d3_manbasis[1] equivalent to t2d3_manbasis[2]\n");
      ftmp = calc_norm (dsp->t2d3_manbasis.vals[1], d->n_cols);
    }      
    else {
      printf("ok\n");
      if (!gram_schmidt(dsp->t2d3_manbasis.vals[0],  dsp->t2d3_manbasis.vals[2],
        d->n_cols))
        g_printerr("t2d3_manbasis[0] equivalent to t2d3_manbasis[2]\n");
      if (!gram_schmidt(dsp->t2d3_manbasis.vals[1],  dsp->t2d3_manbasis.vals[2],
        d->n_cols))
        g_printerr("t2d3_manbasis[1] equivalent to t2d3_manbasis[2]\n");
      ftmp = calc_norm (dsp->t2d3_manbasis.vals[2], d->n_cols);
    }
    }*/

    /*    while (ftmp < tol) {
	  }*/

    dsp->t2d3_no_dir_flag = false;
    if (cpanel->t2d3.manip_mode == MANIP_RADIAL)
    { /* check if variable is currently visible in plot */
      if ((dsp->t2d3.F.vals[0][dsp->t2d3_manip_var]*
        dsp->t2d3.F.vals[0][dsp->t2d3_manip_var] +
        dsp->t2d3.F.vals[1][dsp->t2d3_manip_var]*
        dsp->t2d3.F.vals[1][dsp->t2d3_manip_var]) < tol)
        dsp->t2d3_no_dir_flag = true; /* no */
      else
      { /* yes: set radial manip direction to be current direction
             of contribution */
        dsp->t2d3_rx = (gdouble) dsp->t2d3.F.vals[0][dsp->t2d3_manip_var];
        dsp->t2d3_ry = (gdouble) dsp->t2d3.F.vals[1][dsp->t2d3_manip_var];
        dtmp1 = sqrt(dsp->t2d3_rx*dsp->t2d3_rx+dsp->t2d3_ry*dsp->t2d3_ry);
        dsp->t2d3_rx /= dtmp1;
        dsp->t2d3_ry /= dtmp1;
      }
    }
  }

}

void
tour2d3_manip(gint p1, gint p2, splotd *sp, GGobiSession *gg) 
{
  displayd *dsp = (displayd *) sp->displayptr;
  GGobiStage *d = dsp->d;
  cpaneld *cpanel = &dsp->cpanel;
  gint actual_nvars = dsp->t2d3.nactive;
  gboolean offscreen = false;
  gdouble phi, cosphi, sinphi, ca, sa, cosm, cospsi, sinpsi;
  gdouble distx, disty, x1, x2, y1, y2;
  gdouble denom = (gdouble) MIN(sp->max.x, sp->max.y)/2.;
  gdouble tol = 0.01;
  gdouble dtmp1, dtmp2;
  gdouble len_motion;
  gint i,j,k;

  /* check if off the plot window */
  if (p1 > sp->max.x || p1 < 0 ||
      p2 > sp->max.y || p2 < 0)
    offscreen = true;

  if (dsp->t2d3_manipvar_inc)
    actual_nvars = dsp->t2d3.nactive-1;

  if (!offscreen) {
    dsp->t2d3_pos1_old = dsp->t2d3_pos1;
    dsp->t2d3_pos2_old = dsp->t2d3_pos2;
  
    dsp->t2d3_pos1 = p1;
    dsp->t2d3_pos2 = p2;

    if (actual_nvars > 1)
    {
      distx = disty = 0;
      if (cpanel->t2d3.manip_mode != MANIP_ANGULAR)
      {
        if (cpanel->t2d3.manip_mode == MANIP_OBLIQUE) 
        {
          distx = dsp->t2d3_pos1 - dsp->t2d3_pos1_old;
          disty = dsp->t2d3_pos2_old - dsp->t2d3_pos2;
        }
        else if (cpanel->t2d3.manip_mode == MANIP_VERT) 
        {
          disty = dsp->t2d3_pos2_old - dsp->t2d3_pos2;
        }
        else if (cpanel->t2d3.manip_mode == MANIP_HOR) 
        {
          distx = dsp->t2d3_pos1 - dsp->t2d3_pos1_old;
        }
        else if (cpanel->t2d3.manip_mode == MANIP_RADIAL) 
        {
          if (dsp->t2d3_no_dir_flag)
          {
            distx = dsp->t2d3_pos1 - dsp->t2d3_pos1_old;
            disty = dsp->t2d3_pos2_old - dsp->t2d3_pos2;
            dsp->t2d3_rx = distx;
            dsp->t2d3_ry = disty; 
            dtmp1 = sqrt(dsp->t2d3_rx*dsp->t2d3_rx+dsp->t2d3_ry*dsp->t2d3_ry);
            dsp->t2d3_rx /= dtmp1;
            dsp->t2d3_ry /= dtmp1;
            dsp->t2d3_no_dir_flag = false;
          }
          distx = (dsp->t2d3_rx*(dsp->t2d3_pos1 - dsp->t2d3_pos1_old) + 
            dsp->t2d3_ry*(dsp->t2d3_pos2_old - dsp->t2d3_pos2))*dsp->t2d3_rx;
          disty = (dsp->t2d3_rx*(dsp->t2d3_pos1 - dsp->t2d3_pos1_old) + 
            dsp->t2d3_ry*(dsp->t2d3_pos2_old - dsp->t2d3_pos2))*dsp->t2d3_ry;
        }
        dtmp1 = (gdouble) (distx*distx+disty*disty);
        len_motion = (gdouble) sqrt(dtmp1);

        if (len_motion < tol) /* just in case, maybe not necessary */
        {
          dsp->t2d3_Rmat2.vals[0][0] = 1.0;
          dsp->t2d3_Rmat2.vals[0][1] = 0.0;
          dsp->t2d3_Rmat2.vals[0][2] = 0.0;
          dsp->t2d3_Rmat2.vals[1][0] = 0.0;
          dsp->t2d3_Rmat2.vals[1][1] = 1.0;
          dsp->t2d3_Rmat2.vals[1][2] = 0.0;
          dsp->t2d3_Rmat2.vals[2][0] = 0.0;
          dsp->t2d3_Rmat2.vals[2][1] = 0.0;
          dsp->t2d3_Rmat2.vals[2][2] = 1.0;
        }
        else
        {
          phi = len_motion / denom;
     
          ca = distx/len_motion;
          sa = disty/len_motion;
      
          cosphi = (gdouble) cos((gdouble) phi);
          sinphi = (gdouble) sin((gdouble) phi);
          cosm = 1.0 - cosphi;
          dsp->t2d3_Rmat2.vals[0][0] = ca*ca*cosphi + sa*sa;
          dsp->t2d3_Rmat2.vals[0][1] = -cosm*ca*sa;
          dsp->t2d3_Rmat2.vals[0][2] = sinphi*ca;
          dsp->t2d3_Rmat2.vals[1][0] = -cosm*ca*sa;
          dsp->t2d3_Rmat2.vals[1][1] = sa*sa*cosphi + ca*ca;
          dsp->t2d3_Rmat2.vals[1][2] = sinphi*sa;
          dsp->t2d3_Rmat2.vals[2][0] = -sinphi*ca;
          dsp->t2d3_Rmat2.vals[2][1] = -sinphi*sa;
          dsp->t2d3_Rmat2.vals[2][2] = cosphi;
        }
      }
      else 
      { /* angular constrained manipulation */
        if (dsp->t2d3_pos1_old != sp->max.x/2 && 
          dsp->t2d3_pos2_old != sp->max.y/2 &&
          dsp->t2d3_pos1 != sp->max.x/2 && 
          dsp->t2d3_pos2 != sp->max.y/2)
        {
          x1 = dsp->t2d3_pos1_old - sp->max.x/2;
          y1 = dsp->t2d3_pos2_old - sp->max.y/2;
          dtmp1 = sqrt(x1*x1+y1*y1);
          x1 /= dtmp1;
          y1 /= dtmp1;
          x2 = dsp->t2d3_pos1 - sp->max.x/2;
          y2 = dsp->t2d3_pos2 - sp->max.y/2;
          dtmp2 = sqrt(x2*x2+y2*y2);
          x2 /= dtmp2;
          y2 /= dtmp2;
          if (dtmp1 > tol && dtmp2 > tol)
          {
            cospsi = x1*x2+y1*y2;
            sinpsi = x1*y2-y1*x2;
          }
          else
          {
            cospsi = 1.;    
            sinpsi = 0.;
          }
        }
        else
        {
          cospsi = 1.;
          sinpsi = 0.;
        }
        dsp->t2d3_Rmat2.vals[0][0] = cospsi;
        dsp->t2d3_Rmat2.vals[0][1] = sinpsi;
        dsp->t2d3_Rmat2.vals[0][2] = 0.;
        dsp->t2d3_Rmat2.vals[1][0] = -sinpsi;
        dsp->t2d3_Rmat2.vals[1][1] = cospsi;
        dsp->t2d3_Rmat2.vals[1][2] = 0.;
        dsp->t2d3_Rmat2.vals[2][0] = 0.;
        dsp->t2d3_Rmat2.vals[2][1] = 0.;
        dsp->t2d3_Rmat2.vals[2][2] = 1.;
      }

      /* Set up the rotation matrix in the 3D manip space */
      for (i=0; i<3; i++) 
        for (j=0; j<3; j++)
        {
          dtmp1 = 0.;
          for (k=0; k<3; k++)
            dtmp1 += (dsp->t2d3_mvar_3dbasis.vals[i][k]*
              dsp->t2d3_Rmat2.vals[k][j]);
          dsp->t2d3_Rmat1.vals[i][j] = dtmp1;
        }
      arrayd_copy(&dsp->t2d3_Rmat1, &dsp->t2d3_mvar_3dbasis);

      norm(dsp->t2d3_mvar_3dbasis.vals[0],3); /* just in case */
      norm(dsp->t2d3_mvar_3dbasis.vals[1],3); /* seems to work ok without */
      norm(dsp->t2d3_mvar_3dbasis.vals[2],3); /* this */
      if (!gram_schmidt(dsp->t2d3_mvar_3dbasis.vals[0], 
        dsp->t2d3_mvar_3dbasis.vals[1], 3))
#ifdef EXCEPTION_HANDLING
        g_printerr("");/*t2d3_mvar[0] equivalent to t2d3_mvar[1]\n");*/
#else
        ;
#endif
      if (!gram_schmidt(dsp->t2d3_mvar_3dbasis.vals[0], 
        dsp->t2d3_mvar_3dbasis.vals[2], 3))
#ifdef EXCEPTION_HANDLING
          g_printerr("");/*t2d3_mvar[0] equivalent to t2d3_mvar[2]\n");*/
#else
          ;
#endif
      if (!gram_schmidt(dsp->t2d3_mvar_3dbasis.vals[1], 
        dsp->t2d3_mvar_3dbasis.vals[2], 3))
#ifdef EXCEPTION_HANDLING
        g_printerr("");/*t2d3_mvar[1] equivalent to t2d3_mvar[2]\n");*/
#else
          ;
#endif

      /* Generate the projection of the data corresponding to 
         the 3D rotation in the manip space. */
      for (j=0; j<d->n_cols; j++)
      {
        dsp->t2d3.F.vals[0][j] = 
          dsp->t2d3_manbasis.vals[0][j]*dsp->t2d3_mvar_3dbasis.vals[0][0] +
          dsp->t2d3_manbasis.vals[1][j]*dsp->t2d3_mvar_3dbasis.vals[0][1] +
          dsp->t2d3_manbasis.vals[2][j]*dsp->t2d3_mvar_3dbasis.vals[0][2];
        dsp->t2d3.F.vals[1][j] = 
          dsp->t2d3_manbasis.vals[0][j]*dsp->t2d3_mvar_3dbasis.vals[1][0] +
          dsp->t2d3_manbasis.vals[1][j]*dsp->t2d3_mvar_3dbasis.vals[1][1] +
          dsp->t2d3_manbasis.vals[2][j]*dsp->t2d3_mvar_3dbasis.vals[1][2];
      }
      norm(dsp->t2d3.F.vals[0], d->n_cols);
      norm(dsp->t2d3.F.vals[1], d->n_cols);
      /*      if (calc_norm(dsp->t2d3.F.vals[0], d->n_cols)>1.01) {
	g_printerr("1 F0 out of bounds\n");
        g_printerr("F0: ");
        for (i=0; i<3; i++)
          g_printerr("%f ",dsp->t2d3.F.vals[0][i]);
        g_printerr("\n");
        g_printerr("F1: ");
        for (i=0; i<3; i++)
          g_printerr("%f ",dsp->t2d3.F.vals[1][i]);
        g_printerr("\n");
        g_printerr("manbasis0: ");
        for (i=0; i<3; i++)
          g_printerr("%f ",dsp->t2d3_manbasis.vals[0][i]);
        g_printerr("\n");
        g_printerr("manbasis1: ");
        for (i=0; i<3; i++)
          g_printerr("%f ",dsp->t2d3_manbasis.vals[1][i]);
        g_printerr("\n");
        g_printerr("manbasis2: ");
        for (i=0; i<3; i++)
          g_printerr("%f ",dsp->t2d3_manbasis.vals[2][i]);
        g_printerr("\n");
        g_printerr("m3dvar0: ");
        for (i=0; i<3; i++)
          g_printerr("%f ",dsp->t2d3_mvar_3dbasis.vals[0][i]);
        g_printerr("\n");
        g_printerr("m3dvar1: ");
        for (i=0; i<3; i++)
          g_printerr("%f ",dsp->t2d3_mvar_3dbasis.vals[1][i]);
        g_printerr("\n");
        g_printerr("m3dvar2: ");
        for (i=0; i<3; i++)
          g_printerr("%f ",dsp->t2d3_mvar_3dbasis.vals[2][i]);
        g_printerr("\n");
        g_printerr("distx %f disty %f\n",distx,disty);
      }
      if (calc_norm(dsp->t2d3.F.vals[1], d->n_cols)>1.01) 
      g_printerr("1 F1 out of bounds\n");*/
      if (!gram_schmidt(dsp->t2d3.F.vals[0], dsp->t2d3.F.vals[1], d->n_cols))
#ifdef EXCEPTION_HANDLING
        g_printerr("");/*t2d3.F[0] equivalent to t2d3.F[2]\n");*/
#else
        ;
#endif

      /*      if (calc_norm(dsp->t2d3.F.vals[0], d->n_cols)>1.0) 
	g_printerr("F0 out of bounds\n");
      if (calc_norm(dsp->t2d3.F.vals[1], d->n_cols)>1.0) 
	g_printerr("F1 out of bounds\n");
      */
    }

    display_tailpipe (dsp, FULL, gg);
    varcircles_refresh (d, gg);
  }
}

void
tour2d3_manip_end(splotd *sp) 
{
  displayd *dsp = (displayd *) sp->displayptr;
  cpaneld *cpanel = &dsp->cpanel;
  GGobiSession *gg = GGobiFromSPlot(sp);

  disconnect_motion_signal (sp);

  arrayd_copy(&dsp->t2d3.F, &dsp->t2d3.Fa);
  zero_tau(dsp->t2d3.tau, 2);
  dsp->t2d3.get_new_target = true;

  /* need to turn on tour? */
  if (!cpanel->t2d3.paused) {
    tour2d3_func (T2D3ON, gg->current_display, gg);

    /*-- whenever motion stops, we need a FULL redraw --*/
    display_tailpipe (gg->current_display, FULL, gg);
  }
}

#undef T2D3ON
#undef T2D3OFF

