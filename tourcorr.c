/* tourcorr.c */

#include <gtk/gtk.h>
#ifdef USE_STRINGS_H
#include <strings.h>
#endif

#include <math.h>

#include "vars.h"
#include "externs.h"

void
alloc_tourcorr (displayd *dsp, ggobid *gg)
{
  datad *d = dsp->d;
  gint nc = d->ncols;

  /* first index is the projection dimensions, second dimension is ncols */
  arrayf_init(&dsp->tcorr1.u0);
  arrayf_alloc(&dsp->tcorr1.u0, nc, nc);

  arrayf_init(&dsp->tcorr1.u1);
  arrayf_alloc(&dsp->tcorr1.u1, nc, nc);

  arrayf_init(&dsp->tcorr1.u);
  arrayf_alloc(&dsp->tcorr1.u, nc, nc);

  arrayf_init(&dsp->tcorr1.v0);
  arrayf_alloc(&dsp->tcorr1.v0, nc, nc);

  arrayf_init(&dsp->tcorr1.v1);
  arrayf_alloc(&dsp->tcorr1.v1, nc, nc);

  arrayf_init(&dsp->tcorr1.v);
  arrayf_alloc(&dsp->tcorr1.v, nc, nc);

  arrayf_init(&dsp->tcorr1.uvevec);
  arrayf_alloc(&dsp->tcorr1.uvevec, nc, nc);

  arrayf_init(&dsp->tcorr1.tv);
  arrayf_alloc(&dsp->tcorr1.tv, nc, nc);

  vectori_init(&dsp->tcorr1.vars);
  vectori_alloc(&dsp->tcorr1.vars, nc);
  vectorf_init(&dsp->tcorr1.lambda);
  vectorf_alloc(&dsp->tcorr1.lambda, nc);
  vectorf_init(&dsp->tcorr1.tau);
  vectorf_alloc(&dsp->tcorr1.tau, nc);
  vectorf_init(&dsp->tcorr1.tinc);
  vectorf_alloc(&dsp->tcorr1.tinc, nc);

  /* manipulation controls */
  arrayf_init(&dsp->tc1_manbasis);
  arrayf_alloc(&dsp->tc1_manbasis,2,nc);
  arrayf_init(&dsp->tc2_manbasis);
  arrayf_alloc(&dsp->tc2_manbasis,2,nc);

  /* first index is the projection dimensions, second dimension is ncols */
  arrayf_init(&dsp->tcorr2.u0);
  arrayf_alloc(&dsp->tcorr2.u0, nc, nc);

  arrayf_init(&dsp->tcorr2.u1);
  arrayf_alloc(&dsp->tcorr2.u1, nc, nc);

  arrayf_init(&dsp->tcorr2.u);
  arrayf_alloc(&dsp->tcorr2.u, nc, nc);

  arrayf_init(&dsp->tcorr2.v0);
  arrayf_alloc(&dsp->tcorr2.v0, nc, nc);

  arrayf_init(&dsp->tcorr2.v1);
  arrayf_alloc(&dsp->tcorr2.v1, nc, nc);

  arrayf_init(&dsp->tcorr2.v);
  arrayf_alloc(&dsp->tcorr2.v, nc, nc);

  arrayf_init(&dsp->tcorr2.uvevec);
  arrayf_alloc(&dsp->tcorr2.uvevec, nc, nc);

  arrayf_init(&dsp->tcorr2.tv);
  arrayf_alloc(&dsp->tcorr2.tv, nc, nc);

  vectori_init(&dsp->tcorr2.vars);
  vectori_alloc(&dsp->tcorr2.vars, nc);
  vectorf_init(&dsp->tcorr2.lambda);
  vectorf_alloc(&dsp->tcorr2.lambda, nc);
  vectorf_init(&dsp->tcorr2.tau);
  vectorf_alloc(&dsp->tcorr2.tau, nc);
  vectorf_init(&dsp->tcorr2.tinc);
  vectorf_alloc(&dsp->tcorr2.tinc, nc);

}

/*-- eliminate the nc columns contained in *cols --*/
void
tourcorr_realloc_down (gint nc, gint *cols, datad *d, ggobid *gg)
{
  displayd *dsp;
  GList *l;
  for (l=gg->displays; l; l=l->next) {
    dsp = (displayd *) l->data;
    if (dsp->d == d) {
      arrayf_delete_cols (&dsp->tcorr1.u0, nc, cols);
      arrayf_delete_cols (&dsp->tcorr1.u1, nc, cols);
      arrayf_delete_cols (&dsp->tcorr1.u, nc, cols);
      arrayf_delete_cols (&dsp->tcorr1.v0, nc, cols);
      arrayf_delete_cols (&dsp->tcorr1.v1, nc, cols);
      arrayf_delete_cols (&dsp->tcorr1.v, nc, cols);
      arrayf_delete_cols (&dsp->tcorr1.uvevec, nc, cols);
      arrayf_delete_cols (&dsp->tcorr1.tv, nc, cols);

      vectori_delete_els (&dsp->tcorr1.vars, nc, cols);
      vectorf_delete_els (&dsp->tcorr1.lambda, nc, cols);
      vectorf_delete_els (&dsp->tcorr1.tau, nc, cols);
      vectorf_delete_els (&dsp->tcorr1.tinc, nc, cols);

      arrayf_delete_cols (&dsp->tc1_manbasis, (gint) 2, cols);
      arrayf_delete_cols (&dsp->tc2_manbasis, (gint) 2, cols);

      arrayf_delete_cols (&dsp->tcorr2.u0, nc, cols);
      arrayf_delete_cols (&dsp->tcorr2.u1, nc, cols);
      arrayf_delete_cols (&dsp->tcorr2.u, nc, cols);
      arrayf_delete_cols (&dsp->tcorr2.v0, nc, cols);
      arrayf_delete_cols (&dsp->tcorr2.v1, nc, cols);
      arrayf_delete_cols (&dsp->tcorr2.v, nc, cols);
      arrayf_delete_cols (&dsp->tcorr2.uvevec, nc, cols);
      arrayf_delete_cols (&dsp->tcorr2.tv, nc, cols);

      vectori_delete_els (&dsp->tcorr2.vars, nc, cols);
      vectorf_delete_els (&dsp->tcorr2.lambda, nc, cols);
      vectorf_delete_els (&dsp->tcorr2.tau, nc, cols);
      vectorf_delete_els (&dsp->tcorr2.tinc, nc, cols);
    }
  }
}

/*-- append columns for a total of nc columns --*/
void
tourcorr_realloc_up (gint nc, datad *d, ggobid *gg)
{
  displayd *dsp;
  GList *l;
  for (l=gg->displays; l; l=l->next) {
    dsp = (displayd *) l->data;
    if (dsp->d == d) {
      arrayf_add_cols (&dsp->tcorr1.u0, nc);
      arrayf_add_cols (&dsp->tcorr1.u1, nc);
      arrayf_add_cols (&dsp->tcorr1.u, nc);
      arrayf_add_cols (&dsp->tcorr1.v0, nc);
      arrayf_add_cols (&dsp->tcorr1.v1, nc);
      arrayf_add_cols (&dsp->tcorr1.v, nc);
      arrayf_add_cols (&dsp->tcorr1.uvevec, nc);
      arrayf_add_cols (&dsp->tcorr1.tv, nc);

      vectori_realloc (&dsp->tcorr1.vars, nc);
      vectorf_realloc (&dsp->tcorr1.lambda, nc);
      vectorf_realloc (&dsp->tcorr1.tau, nc);
      vectorf_realloc (&dsp->tcorr1.tinc, nc);

      arrayf_add_cols (&dsp->tc1_manbasis, (gint) 2);
      arrayf_add_cols (&dsp->tc2_manbasis, (gint) 2);

      arrayf_add_cols (&dsp->tcorr2.u0, nc);
      arrayf_add_cols (&dsp->tcorr2.u1, nc);
      arrayf_add_cols (&dsp->tcorr2.u, nc);
      arrayf_add_cols (&dsp->tcorr2.v0, nc);
      arrayf_add_cols (&dsp->tcorr2.v1, nc);
      arrayf_add_cols (&dsp->tcorr2.v, nc);
      arrayf_add_cols (&dsp->tcorr2.uvevec, nc);
      arrayf_add_cols (&dsp->tcorr2.tv, nc);

      vectori_realloc (&dsp->tcorr2.vars, nc);
      vectorf_realloc (&dsp->tcorr2.lambda, nc);
      vectorf_realloc (&dsp->tcorr2.tau, nc);
      vectorf_realloc (&dsp->tcorr2.tinc, nc);
    }
  }
}

void
free_tourcorr(displayd *dsp)
{
  /*  gint k;*/
  /*  datad *d = dsp->d;*/
  /*  gint nc = d->ncols;*/

  vectori_free(&dsp->tcorr1.vars);
  vectorf_free(&dsp->tcorr1.lambda);
  vectorf_free(&dsp->tcorr1.tau);
  vectorf_free(&dsp->tcorr1.tinc);

  arrayf_free(&dsp->tcorr1.u0, 0, 0);
  arrayf_free(&dsp->tcorr1.u1, 0, 0);
  arrayf_free(&dsp->tcorr1.u, 0, 0);

  arrayf_free(&dsp->tcorr1.v0, 0, 0);
  arrayf_free(&dsp->tcorr1.v1, 0, 0);
  arrayf_free(&dsp->tcorr1.v, 0, 0);

  arrayf_free(&dsp->tcorr1.uvevec, 0, 0);
  arrayf_free(&dsp->tcorr1.tv, 0, 0);

  arrayf_free(&dsp->tc1_manbasis, 0, 0);
  arrayf_free(&dsp->tc2_manbasis, 0, 0);

  vectori_free(&dsp->tcorr2.vars);
  vectorf_free(&dsp->tcorr2.lambda);
  vectorf_free(&dsp->tcorr2.tau);
  vectorf_free(&dsp->tcorr2.tinc);

  arrayf_free(&dsp->tcorr2.u0, 0, 0);
  arrayf_free(&dsp->tcorr2.u1, 0, 0);
  arrayf_free(&dsp->tcorr2.u, 0, 0);

  arrayf_free(&dsp->tcorr2.v0, 0, 0);
  arrayf_free(&dsp->tcorr2.v1, 0, 0);
  arrayf_free(&dsp->tcorr2.v, 0, 0);

  arrayf_free(&dsp->tcorr2.uvevec, 0, 0);
  arrayf_free(&dsp->tcorr2.tv, 0, 0);

}

void 
display_tourcorr_init (displayd *dsp, ggobid *gg) {
  gint i, j;
  datad *d = dsp->d;
  cpaneld *cpanel = &dsp->cpanel;
  gint nc = d->ncols;

  alloc_tourcorr(dsp, gg);
 
  /* Initialize first variable as the vertical and rest of the variables as
     the horizontal variables */
  dsp->tcorr1.nvars = 2;
  dsp->tcorr1.vars.els[0] = 0;
  dsp->tcorr1.vars.els[1] = 1;

  dsp->tcorr2.nvars = nc-2;
  for (j=0; j<nc-2; j++)
    dsp->tcorr2.vars.els[j] = j+2;

  /* declare starting vertical base as first variable */
  for (i=0; i<nc; i++)
    for (j=0; j<nc; j++)
      dsp->tcorr1.u0.vals[i][j] = dsp->tcorr1.u1.vals[i][j] = 
        dsp->tcorr1.u.vals[i][j] = 
        dsp->tcorr1.v0.vals[i][j] = dsp->tcorr1.v1.vals[i][j] = 0.0;

  dsp->tcorr1.u.vals[0][dsp->tcorr1.vars.els[0]] = 1.0;

  for (i=0; i<nc; i++)
    for (j=0; j<nc; j++)
      dsp->tcorr2.u0.vals[i][j] = dsp->tcorr2.u1.vals[i][j] = 
        dsp->tcorr2.u.vals[i][j] = 
        dsp->tcorr2.v0.vals[i][j] = dsp->tcorr2.v1.vals[i][j] = 0.0;

  dsp->tcorr2.u.vals[0][dsp->tcorr2.vars.els[0]] = 1.0;

  dsp->tcorr1.dv = 1.0;
  dsp->tcorr1.delta = cpanel->tcorr1_step*M_PI_2/10.0;
  dsp->tcorr1.nsteps = 1; 
  dsp->tcorr1.stepcntr = 1;

  dsp->tcorr1.idled = 0;
  dsp->tcorr1.get_new_target = true;

  dsp->tcorr2.dv = 1.0;
  dsp->tcorr2.delta = cpanel->tcorr2_step*M_PI_2/10.0;
  dsp->tcorr2.nsteps = 1; 
  dsp->tcorr2.stepcntr = 1;

  dsp->tcorr2.idled = 0;
  dsp->tcorr2.get_new_target = true;
}

void tourcorr_speed_set(gint slidepos, ggobid *gg) {
  displayd *dsp = gg->current_display; 
  cpaneld *cpanel = &dsp->cpanel;
  extern void speed_set (gint, gfloat *, gfloat *, gfloat, gint *, gint *);

  speed_set(slidepos, &cpanel->tcorr1_step, &dsp->tcorr1.delta,  
    dsp->tcorr1.dv, &dsp->tcorr1.nsteps, &dsp->tcorr1.stepcntr);

  speed_set(slidepos, &cpanel->tcorr2_step, &dsp->tcorr2.delta,  
    dsp->tcorr2.dv, &dsp->tcorr2.nsteps, &dsp->tcorr2.stepcntr);
}

void 
tourcorr_horvar_set (gint jvar, ggobid *gg)
{
  gint j, jtmp, k;
  gboolean active=false;
  displayd *dsp = gg->current_display;

  for (j=0; j<dsp->tcorr1.nvars; j++)
    if (jvar == dsp->tcorr1.vars.els[j])
      active = true;

  /* deselect var if tcorr1.nvars > 1 */
  if (active) {
    if (dsp->tcorr1.nvars > 1) {
      for (j=0; j<dsp->tcorr1.nvars; j++) {
        if (jvar == dsp->tcorr1.vars.els[j]) 
          break;
      }
      if (j<dsp->tcorr1.nvars-1) {
        for (k=j; k<dsp->tcorr1.nvars-1; k++){
          dsp->tcorr1.vars.els[k] = dsp->tcorr1.vars.els[k+1];
	}
      }
      dsp->tcorr1.nvars--;
      /*      printf("active %d: ",dsp->tcorr1.nvars);
      for (j=0; j<dsp->tcorr1.nvars; j++)
        printf("%d ",dsp->tcorr1.vars.els[j]);
	printf("\n");*/
    }
  }
  else { /* not active, so add the variable */
    if (jvar > dsp->tcorr1.vars.els[dsp->tcorr1.nvars-1]) {
      dsp->tcorr1.vars.els[dsp->tcorr1.nvars] = jvar;
    }
    else if (jvar < dsp->tcorr1.vars.els[0]) {
      for (j=dsp->tcorr1.nvars; j>0; j--) {
          dsp->tcorr1.vars.els[j] = dsp->tcorr1.vars.els[j-1];
      }
      dsp->tcorr1.vars.els[0] = jvar;
    }
    else {
      for (j=0; j<dsp->tcorr1.nvars-1; j++) {
        if (jvar > dsp->tcorr1.vars.els[j] && jvar < 
          dsp->tcorr1.vars.els[j+1]) {
          jtmp = j+1;
          break;
	}
      }
      for (j=dsp->tcorr1.nvars-1;j>=jtmp; j--) 
          dsp->tcorr1.vars.els[j+1] = dsp->tcorr1.vars.els[j];
      dsp->tcorr1.vars.els[jtmp] = jvar;
    }
    dsp->tcorr1.nvars++;
    /*    printf("not active %d: ",dsp->tcorr1.nvars);
    for (j=0; j<dsp->tcorr1.nvars; j++)
      printf("%d ",dsp->tcorr1.vars.els[j]);
      printf("\n");*/
  }

  dsp->tcorr1.get_new_target = true;

}

void 
tourcorr_vervar_set (gint jvar, ggobid *gg)
{
  gint j, jtmp, k;
  gboolean active=false;
  displayd *dsp = gg->current_display;

  for (j=0; j<dsp->tcorr2.nvars; j++)
    if (jvar == dsp->tcorr2.vars.els[j])
      active = true;

  /* deselect var if tcorr2.nvars > 1 */
  if (active) {
    if (dsp->tcorr2.nvars > 1) {
      for (j=0; j<dsp->tcorr2.nvars; j++) {
        if (jvar == dsp->tcorr2.vars.els[j]) 
          break;
      }
      if (j<dsp->tcorr2.nvars-1) {
        for (k=j; k<dsp->tcorr2.nvars-1; k++){
          dsp->tcorr2.vars.els[k] = dsp->tcorr2.vars.els[k+1];
	}
      }
      dsp->tcorr2.nvars--;
    }
  }
  else { /* not active, so add the variable */
    if (jvar > dsp->tcorr2.vars.els[dsp->tcorr2.nvars-1]) {
      dsp->tcorr2.vars.els[dsp->tcorr2.nvars] = jvar;
    }
    else if (jvar < dsp->tcorr2.vars.els[0]) {
      for (j=dsp->tcorr2.nvars; j>0; j--) {
          dsp->tcorr2.vars.els[j] = dsp->tcorr2.vars.els[j-1];
      }
      dsp->tcorr2.vars.els[0] = jvar;
    }
    else {
      for (j=0; j<dsp->tcorr2.nvars-1; j++) {
        if (jvar > dsp->tcorr2.vars.els[j] && jvar < dsp->tcorr2.vars.els[j+1]) {
          jtmp = j+1;
          break;
	}
      }
      for (j=dsp->tcorr2.nvars-1;j>=jtmp; j--) 
          dsp->tcorr2.vars.els[j+1] = dsp->tcorr2.vars.els[j];
      dsp->tcorr2.vars.els[jtmp] = jvar;
    }
    dsp->tcorr2.nvars++;
  }

  dsp->tcorr2.get_new_target = true;
}

static void
tourcorr_manip_var_set (gint j, ggobid *gg)
{
  g_printerr ("set the manipulation variable; not yet implemented\n");
}

void
tourcorr_varsel (gint jvar, gint button, datad *d, ggobid *gg)
{
  if (button == 1 || button == 2) {
    if (d->vcirc_ui.jcursor == GDK_HAND2)
      tourcorr_manip_var_set (jvar, gg);

    else {
      if (button == 1) { 
        tourcorr_horvar_set (jvar, gg);
      }
      else if ( button == 2) {
        tourcorr_vervar_set (jvar, gg);
      }
    }
  }
}

void
tourcorr_projdata(splotd *sp, glong **world_data, datad *d, ggobid *gg) {
  gint i, j, m;
  displayd *dsp = (displayd *) sp->displayptr;

  for (m=0; m<d->nrows_in_plot; m++)
  {
    i = d->rows_in_plot[m];
    sp->planar[i].x = 0;
    sp->planar[i].y = 0;
    for (j=0; j<d->ncols; j++)
    {
      sp->planar[i].x += (gint)(dsp->tcorr1.u.vals[0][j]*world_data[i][j]);
      sp->planar[i].y += (gint)(dsp->tcorr2.u.vals[0][j]*world_data[i][j]);
    }
  }
}

void
tourcorr_run(displayd *dsp, ggobid *gg)
{
  datad *d = dsp->d;
  gint i;
  extern gboolean reached_target(gint, gint);
  extern void increment_tour(vector_f, vector_f, gint *, gint *, gfloat, 
    gfloat, gint);
  extern void do_last_increment(vector_f, vector_f, gint);
  extern void gt_basis(array_f, gint, vector_i, gint, gint);
  extern void path(array_f, array_f, array_f, gint, gint, array_f, 
    array_f, array_f, vector_f, array_f, array_f,
    vector_f, vector_f, gint *, gint *, gfloat *, gfloat);
  extern void tour_reproject(vector_f, array_f, array_f, array_f, 
    array_f, array_f, gint, gint);
  extern void copy_mat(gfloat **, gfloat **, gint, gint);

  if (!dsp->tcorr1.get_new_target && 
      !reached_target(dsp->tcorr1.nsteps, dsp->tcorr1.stepcntr)) {

    /*  if (!dsp->tcorr1.get_new_target && !dsp->tcorr2.get_new_target && 
       (!reached_target(dsp->tcorr1.nsteps, dsp->tcorr1.stepcntr) ||
       !reached_target(dsp->tcorr2.nsteps, dsp->tcorr2.stepcntr))) {*/

    increment_tour(dsp->tcorr1.tinc, dsp->tcorr1.tau, &dsp->tcorr1.nsteps, 
      &dsp->tcorr1.stepcntr, dsp->tcorr1.dv, dsp->tcorr1.delta, (gint) 1);

    tour_reproject(dsp->tcorr1.tinc, dsp->tcorr1.v, dsp->tcorr1.v0, 
      dsp->tcorr1.v1, dsp->tcorr1.u, dsp->tcorr1.uvevec, d->ncols, (gint) 1);
  }
  else { /* do final clean-up and get new target */
    if (!dsp->tcorr1.get_new_target) {
      do_last_increment(dsp->tcorr1.tinc, dsp->tcorr1.tau, (gint) 1);
      tour_reproject(dsp->tcorr1.tinc, dsp->tcorr1.v, dsp->tcorr1.v0, 
        dsp->tcorr1.v1, dsp->tcorr1.u, dsp->tcorr1.uvevec, d->ncols, (gint) 1);
      }
    
    copy_mat(dsp->tcorr1.u0.vals, dsp->tcorr1.u.vals, d->ncols, 1);

    gt_basis(dsp->tcorr1.u1, dsp->tcorr1.nvars, dsp->tcorr1.vars, d->ncols, 
      (gint) 1);
    path(dsp->tcorr1.u0, dsp->tcorr1.u1, dsp->tcorr1.u, d->ncols, (gint) 1, 
       dsp->tcorr1.v0, dsp->tcorr1.v1, dsp->tcorr1.v, dsp->tcorr1.lambda, 
       dsp->tcorr1.tv, dsp->tcorr1.uvevec,
       dsp->tcorr1.tau, dsp->tcorr1.tinc, &dsp->tcorr1.nsteps, 
       &dsp->tcorr1.stepcntr, 
       &dsp->tcorr1.dv, dsp->tcorr1.delta);

    dsp->tcorr1.get_new_target = false;
  }

  if (!dsp->tcorr2.get_new_target && 
      !reached_target(dsp->tcorr2.nsteps, dsp->tcorr2.stepcntr)) {
    increment_tour(dsp->tcorr2.tinc, dsp->tcorr2.tau, &dsp->tcorr2.nsteps, 
      &dsp->tcorr2.stepcntr, dsp->tcorr2.dv, dsp->tcorr2.delta, (gint) 1);

    tour_reproject(dsp->tcorr2.tinc, dsp->tcorr2.v, dsp->tcorr2.v0, 
      dsp->tcorr2.v1, dsp->tcorr2.u, dsp->tcorr2.uvevec, d->ncols, (gint) 1);
  }
  else { /* do final clean-up and get new target */
    if (!dsp->tcorr2.get_new_target) {
      do_last_increment(dsp->tcorr2.tinc, dsp->tcorr2.tau, (gint) 1);
      tour_reproject(dsp->tcorr2.tinc, dsp->tcorr2.v, dsp->tcorr2.v0, 
        dsp->tcorr2.v1, dsp->tcorr2.u, dsp->tcorr2.uvevec, d->ncols, (gint) 1);
    }
    copy_mat(dsp->tcorr2.u0.vals, dsp->tcorr2.u.vals, d->ncols, 1);

    gt_basis(dsp->tcorr2.u1, dsp->tcorr2.nvars, dsp->tcorr2.vars, d->ncols, 
      (gint) 1);
    path(dsp->tcorr2.u0, dsp->tcorr2.u1, dsp->tcorr2.u, d->ncols, (gint) 1, 
      dsp->tcorr2.v0, dsp->tcorr2.v1, dsp->tcorr2.v, dsp->tcorr2.lambda, 
      dsp->tcorr2.tv, dsp->tcorr2.uvevec,
      dsp->tcorr2.tau, dsp->tcorr2.tinc, &dsp->tcorr2.nsteps, 
      &dsp->tcorr2.stepcntr, &dsp->tcorr2.dv, dsp->tcorr2.delta);

    dsp->tcorr2.get_new_target = false;
  }
  
  display_tailpipe (dsp, gg);

  varcircles_refresh (d, gg);
}

void
tourcorr_do_step(displayd *dsp, ggobid *gg)
{
  tourcorr_run(dsp, gg);
}

gint
tourcorr_idle_func (ggobid *gg)
{
  displayd *dsp = gg->current_display;
  cpaneld *cpanel = &dsp->cpanel;
  gboolean doit = !cpanel->tcorr1_paused;

  if (doit) {
    tourcorr_run(dsp, gg);
    gdk_flush ();
  }

  return (doit);
}

void tourcorr_func (gboolean state, displayd *dsp, ggobid *gg)
{
  if (state) {
    dsp->tcorr1.idled = gtk_idle_add_priority (G_PRIORITY_LOW,
                                   (GtkFunction) tourcorr_idle_func, gg);
    gg->tourcorr.idled = 1;
  } else {
    gtk_idle_remove (dsp->tcorr1.idled);
    gg->tourcorr.idled = 0;
  }

/*
   if (state)
     tour_idle = gtk_timeout_add (40, tour_idle_func, NULL);
   else
     gtk_timeout_remove (tour_idle);
*/
}

void tourcorr_reinit(ggobid *gg)
{
  int j, m;
  displayd *dsp = gg->current_display;
  datad *d = dsp->d;
  extern void tour_reproject(vector_f, array_f, array_f, array_f, 
    array_f, array_f, gint, gint);
  extern void zero_tinc(vector_f, gint);
  extern void zero_tau(vector_f, gint);
  extern void zero_lambda(vector_f, gint);

  printf(" in reinit\n");
  for (j=0; j<d->ncols; j++) {
    /*    m = dsp->tcorr1.vars.els[j];*/
    /*    dsp->tcorr1.u0.vals[0][j] = 0.;*/
    dsp->tcorr1.u.vals[0][j] = 0.;
    dsp->tcorr1.v0.vals[0][j] = 0.;
    dsp->tcorr1.u0.vals[0][j] = 0.;
    dsp->tcorr1.v1.vals[0][j] = 0.;
    dsp->tcorr1.u1.vals[0][j] = 0.;
  }
  m = dsp->tcorr1.vars.els[0];
  /*  dsp->tcorr1.u0.vals[0][m] = 1.;*/
  dsp->tcorr1.u.vals[0][m] = 1.;
  dsp->tcorr1.u0.vals[0][m] = 1.;
  dsp->tcorr1.v0.vals[0][m] = 1.;

  dsp->tcorr1.get_new_target = true;

  for (j=0; j<d->ncols; j++) {
    dsp->tcorr2.u.vals[0][j] = 0.;
    dsp->tcorr2.u0.vals[0][j] = 0.;
    dsp->tcorr2.v0.vals[0][j] = 0.;
    dsp->tcorr2.u1.vals[0][j] = 0.;
    dsp->tcorr2.v1.vals[0][j] = 0.;
  }
  m = dsp->tcorr2.vars.els[0];
  /*  dsp->tcorr2.u0.vals[0][m] = 1.;*/
  dsp->tcorr2.u.vals[0][m] = 1.;
  dsp->tcorr2.u0.vals[0][m] = 1.;
  dsp->tcorr2.v0.vals[0][m] = 1.;

  dsp->tcorr2.get_new_target = true;

  zero_tinc(dsp->tcorr1.tinc, 1);
  zero_tinc(dsp->tcorr2.tinc, 1);
  zero_tau(dsp->tcorr1.tau, 1);
  zero_tau(dsp->tcorr2.tau, 1);
  zero_lambda(dsp->tcorr1.lambda, 1);
  zero_lambda(dsp->tcorr2.lambda, 1);

  dsp->tcorr1.nsteps = 0;
  dsp->tcorr2.nsteps = 0;
  dsp->tcorr1.stepcntr = 0;
  dsp->tcorr2.stepcntr = 0;

  display_tailpipe (dsp, gg);

  varcircles_refresh (d, gg);

}

/* Variable manipulation */
void
tourcorr_manip_init(gint p1, gint p2, splotd *sp) 
{
  displayd *dsp = (displayd *) sp->displayptr;
  datad *d = dsp->d;
  gint j;
  gint n1vars = dsp->tcorr1.nvars, n2vars = dsp->tcorr2.nvars;
  extern void gram_schmidt(gfloat *, gfloat*, gint);
  extern void copy_mat(gfloat **, gfloat **, gint, gint);

  dsp->tc1_pos = dsp->tc1_pos_old = p1;
  dsp->tc2_pos = dsp->tc2_pos_old = p2;
  dsp->tc1_manipvar_inc = false;
  dsp->tc2_manipvar_inc = false;

  /* need to turn off tour? */
  for (j=0; j<dsp->tcorr1.nvars; j++)
    if (dsp->tcorr1.vars.els[j] == dsp->tc1_manip_var) {
      dsp->tc1_manipvar_inc = true;
      n1vars--;
    }
  for (j=0; j<dsp->tcorr2.nvars; j++)
    if (dsp->tcorr2.vars.els[j] == dsp->tc2_manip_var) {
      dsp->tc2_manipvar_inc = true;
      n2vars--;
    }
  /*  printf("manip var %d n1vars %d \n",dsp->tc1_manip_var,n1vars);*/
  /* make manip basis */
  for (j=0; j<d->ncols; j++) {
    dsp->tc1_manbasis.vals[0][j] = dsp->tcorr1.u.vals[0][j];
    dsp->tc1_manbasis.vals[1][j] = 0.;
  }
  for (j=0; j<d->ncols; j++) {
    dsp->tc2_manbasis.vals[0][j] = dsp->tcorr2.u.vals[0][j];
    dsp->tc2_manbasis.vals[1][j] = 0.;
  }
  dsp->tc1_manbasis.vals[1][dsp->tc1_manip_var]=1.;
  dsp->tc2_manbasis.vals[1][dsp->tc2_manip_var]=1.;
  /*  for (j=0; j<d->ncols; j++)
  {
  printf("man basis %f %f\n",dsp->tc1_manbasis.vals[0][j],dsp->tc2_manbasis.vals[0][j]);*/
    /*    dsp->tcorr1.u.vals[0][j] = dsp->tc1_manbasis.vals[0][j];
	  dsp->tcorr2.u.vals[0][j] = dsp->tc2_manbasis.va1s[0][j];*/
  /*  }*/
  if (n1vars > 0)
  {
    gram_schmidt(dsp->tc1_manbasis.vals[1],  dsp->tc1_manbasis.vals[0],
      d->ncols);
    /*    ftmp = calc_norm(dsp->tc1_manbasis.vals[1], d->ncols);
    while (ftmp < tol)
    {
      gen_norm_variates(1, xg->ncorr_xvars, xg->tv[0]);
      for (j=0; j<d->ncols; j++)
        dsp->tc1_manbasis.vals[1][j] = 0.;
      for (j=0; j<xg->ncorr_xvars; j++)
        dsp->tc1_manbasis.vals[1][xg->corr_xvars[j]] = xg->tv[0][j];
      norm(dsp->tc1_manbasis.vals[1], d->ncols);
      gram_schmidt(dsp->tc1_manbasis.vals[0],  dsp->tc1_manbasis.vals[1],
        d->ncols);
      ftmp = calc_norm(dsp->tc1_manbasis.vals[1], d->ncols);
      }*/
  }
  if (n2vars > 0)
  {
    gram_schmidt(dsp->tc2_manbasis.vals[1],  dsp->tc2_manbasis.vals[0],
      d->ncols);
    /*    ftmp = calc_norm(dsp->tc2_manbasis.vals[1], d->ncols);
    while (ftmp < tol)
    {
      gen_norm_variates(1, xg->ncorr_yvars, xg->tv[0]);
      for (j=0; j<d->ncols; j++)
        dsp->tc2_manbasis.vals[1][j] = 0.;
      for (j=0; j<xg->ncorr_yvars; j++)
        dsp->tc2_manbasis.vals[1][xg->corr_yvars[j]] = xg->tv[0][j];
      norm(dsp->tc2_manbasis.vals[1], d->ncols);
      gram_schmidt(dsp->tc2_manbasis.vals[0],  dsp->tc2_manbasis.vals[1],
        d->ncols);
      ftmp = calc_norm(dsp->tc2_manbasis.vals[1], d->ncols);
      }*/
  } 
  copy_mat(dsp->tcorr1.u.vals, dsp->tcorr1.u0.vals, d->ncols, 1);
  copy_mat(dsp->tcorr2.u.vals, dsp->tcorr2.u0.vals, d->ncols, 1);

}

void
tourcorr_manip(gint p1, gint p2, splotd *sp, ggobid *gg) 
     /*tourcorr_manip(gint p1, gint p2, splotd *sp) */
{
  displayd *dsp = (displayd *) sp->displayptr;
  datad *d = dsp->d;
  gfloat xphi = 0., yphi=0., xcosphi=1., xsinphi=0., ycosphi=1., ysinphi=0.;
  gfloat distx, disty;
  gfloat denom = (float) MIN(sp->max.x, sp->max.y)/2.;
  gint actual_nxvars = dsp->tcorr1.nvars, actual_nyvars = dsp->tcorr2.nvars;
  gint j;

  printf("denom %f sp dim %d %d \n",denom,sp->max.x,sp->max.y);

  if (dsp->tc1_manipvar_inc)
    actual_nxvars = dsp->tcorr1.nvars-1;
  if (dsp->tc2_manipvar_inc)
    actual_nyvars = dsp->tcorr2.nvars-1;

  dsp->tc1_pos_old = dsp->tc1_pos;
  dsp->tc2_pos_old = dsp->tc2_pos;

  dsp->tc1_pos = p1;
  dsp->tc2_pos = p2;

  /*  printf("%d %d %d %d\n",dsp->tc1_pos_old, dsp->tc1_pos, dsp->tc2_pos_old, 
    dsp->tc2_pos);
    printf("%d %d\n",actual_nxvars, actual_nyvars);*/

  if (actual_nxvars > 0 || actual_nyvars > 0)
  {
    if (dsp->tc_manip_mode == CMANIP_VERT)
    {
      distx = 0.;
      if (actual_nyvars > 0)
        disty = dsp->tc2_pos_old - dsp->tc2_pos;
    }
    else if (dsp->tc_manip_mode == CMANIP_HOR)
    {
      if (actual_nxvars > 0)
        distx = dsp->tc1_pos - dsp->tc1_pos_old;
      disty = 0.;
    }
    else if (dsp->tc_manip_mode == CMANIP_COMB)
    {
      if (actual_nxvars > 0)
        distx = dsp->tc1_pos - dsp->tc1_pos_old;
      if (actual_nyvars > 0)
        disty = dsp->tc2_pos_old - dsp->tc2_pos;
    }
    else if (dsp->tc_manip_mode == CMANIP_EQUAL)
    {
      if (actual_nxvars > 0)
        distx = dsp->tc1_pos - dsp->tc1_pos_old;
      if (actual_nyvars > 0)
        disty = dsp->tc2_pos_old - dsp->tc2_pos;
      if (fabs(distx) != fabs(disty))
      {
        distx = (distx+disty)/1.414214;
        disty = distx;
      }
    }
    
    xphi = xphi + distx / denom;
    yphi = yphi + disty / denom;
 
    xcosphi = (gfloat) cos((gdouble) xphi);
    xsinphi = (gfloat) sin((gdouble) xphi);
    if (xcosphi > 1.0)
    {
      xcosphi = 1.0;
      xsinphi = 0.0;
    }
    else if (xcosphi < -1.0)
    {
      xcosphi = -1.0;
      xsinphi = 0.0;
    }
    ycosphi = (float) cos((double) yphi);
    ysinphi = (float) sin((double) yphi);
    if (ycosphi > 1.0)
    {
      ycosphi = 1.0;
      ysinphi = 0.0;
    }
    else if (ycosphi < -1.0)
    {
      ycosphi = -1.0;
      ysinphi = 0.0;
    }
  }

  printf("phi %f %f %f %f dist %f %f\n",xphi,yphi,xcosphi,xsinphi,distx,disty);

  /* generate the projection basis */
  if (actual_nxvars > 0) 
  {
    for (j=0; j<d->ncols; j++)
      dsp->tcorr1.u.vals[0][j] = xcosphi * dsp->tc1_manbasis.vals[1][j] + 
       xsinphi * dsp->tc1_manbasis.vals[0][j];
    printf("manip x\n");
  }

  if (actual_nyvars > 0)
  {
    for (j=0; j<d->ncols; j++)
      dsp->tcorr2.u.vals[1][j] = ycosphi * dsp->tc2_manbasis.vals[1][j] + 
       ysinphi * dsp->tc2_manbasis.vals[0][j];
    printf("manip y\n");
  }

  for (j=0; j<d->ncols; j++)
  {
    printf("u %f %f\n",dsp->tcorr1.u.vals[0][j],dsp->tcorr2.u.vals[0][j]);
    }
  display_tailpipe (dsp, gg);

  /*  varcircles_refresh (d, gg);*/
}

void
tourcorr_manip_end(splotd *sp) 
{
  displayd *dsp = (displayd *) sp->displayptr;
  datad *d = dsp->d;
  extern void copy_mat(gfloat **, gfloat **, gint, gint);

  copy_mat(dsp->tcorr1.u.vals, dsp->tcorr1.u0.vals, d->ncols, 1);
  copy_mat(dsp->tcorr2.u.vals, dsp->tcorr2.u0.vals, d->ncols, 1);
}
