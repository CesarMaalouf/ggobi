/*-- types.h --*/
/*
    This software may only be used by you under license from AT&T Corp.
    ("AT&T").  A copy of AT&T's Source Code Agreement is available at
    AT&T's Internet website having the URL:
    <http://www.research.att.com/areas/stat/ggobi/license.html>
    If you received this software without first entering into a license
    with AT&T, you have an infringing copy of this software and cannot use
    it without violating AT&T's intellectual property rights.
*/
#ifndef TYPES_H
#define TYPES_H

#include <gtk/gtk.h>
/* types.h */

typedef enum {ascii_data, binary_data, Sprocess_data, xml_data, mysql_data, unknown_data, num_data_modes} DataMode;
typedef enum {PLUS=1, X, OR, FR, OC, FC, DOT,UNKNOWN_GLYPH} GlyphType;

typedef enum { RESET_UNHIDE_POINTS, RESET_POINT_COLORS, RESET_GLYPHS, 
               RESET_UNHIDE_LINES, RESET_LINES, RESET_INIT_BRUSH} BrushReset;

typedef struct {
    glong x, y;
} lcoords;
typedef struct {
    gint x, y;
} icoords;
typedef struct {
    gfloat x, y;
} fcoords;
typedef struct {
    gfloat min, max;
} lims;
typedef struct {
    gint a, b;
} endpointsd;
typedef struct {
    gint x1, y1, x2, y2;
} brush_coords;

/*-- arrays --*/
/*-- floating point: for gg.raw_data, tform1, tform2 --*/
typedef struct {
  gfloat **vals;
  guint nrows, ncols;
} array_f;
/*-- short: for gg.missing --*/
typedef struct {
  gshort **vals;
  guint nrows, ncols;
} array_s;
/*-- long: for gg.world, jitdata --*/
typedef struct {
  glong **vals;
  guint nrows, ncols;
} array_l;

/*-- vectors --*/
typedef struct {
  gfloat *els;
  guint nels;
} vector_f;
typedef struct {
  gint *els;
  guint nels;
} vector_i;
typedef struct {
  gshort *els;
  guint nels;
} vector_s;
typedef struct {
  gboolean *els;
  guint nels;
} vector_b;

typedef struct {  /*-- used for obtaining ranks --*/
  gfloat f;
  gint indx;
} paird;

/* column-wise data that will appear in the variable table */
typedef struct {
 gchar *collab, *collab_tform;
 gint nmissing;

 /*-- reference variable:  jref=-1 except for cloned or sphered variables --*/
 gint jref;

 /*-- unadjusted, unaffected by imputation --*/
 gfloat mean, median;

 lims lim_raw;       /*-- range of the raw data          --*/
 lims lim_tform;     /*-- range of d->tform              --*/

 /*
  * If the user has supplied limits, lim_specified_p = true
  * and the limits are stored in lim_specified.{min,max}
 */
 gboolean lim_specified_p;
 lims lim_specified;
 lims lim_specified_tform;

 lims lim;      /*-- limits in use: lim_specified_tform or lim_tform --*/

 /*-- transformations --*/
 gint tform0;
 gfloat domain_incr;  /*-- stage 0 --*/
 gfloat (*domain_adj) (gfloat x, gfloat incr);
 gfloat (*inv_domain_adj) (gfloat x, gfloat incr);
 gint tform1;
 gfloat param;
 gint tform2;

 /*-- jittering --*/
 gfloat jitter_factor;

 /*-- in variable table --*/
 gboolean selected;

} vartabled;


/* these data types seem to be missing in gdk */
typedef struct {
  gint x, y;
  gint width, height;
} arcd; 
typedef struct {
  gint x, y;
  gint width, height;
} rectd; 

/* cluster; to be used in Group Exclusion tool */
typedef struct {
  glong n;  /*-- Can I know the number of elements in this cluster? --*/
  gint glyphtype, glyphsize;
  gshort color;
  gboolean hidden, included;
  GtkWidget *da, *lbl, *hide_tgl, *exclude_tgl;
} clusterd;

/* glyph vectors */
typedef struct {
  gint type;
  gint size;
} glyphv;

/* structure for binning the plot window */
typedef struct {
  gulong *els;
  gint nels;
  gint nblocks;  /* how many blocks have been allocated for els */
} bin_struct;

/* row groups */
typedef struct {
  gint id, nels, *els;
  gboolean included;  /* for linked brushing */
  gboolean sampled;   /* for subsetting */
} rgroupd;


/*-- used to keep track of history in moving points --*/
typedef struct {
  gint i, j;
  gfloat val;
} celld;

/*-- tour elements --*/
typedef struct {
  gint nvars;
  vector_i vars;
  array_f u0, u1, u, uold, v0, v1, v, uvevec, tv;
  vector_f lambda, tau, tinc;
  gfloat dv, delta;
  gint idled;
  gboolean get_new_target;
  gint nsteps, stepcntr;
} tour;

/*
 * display options
*/
typedef struct {
 gboolean points_show_p;              /* scatterplot, scatmat, parcoords */
 gboolean edges_directed_show_p;      /* scatterplot, scatmat */
 gboolean edges_undirected_show_p;    /* scatterplot, scatmat */
 gboolean edges_show_p;               /* parcoords */
 gboolean missings_show_p;            /* scatterplot, scatmat, parcoords */

 gboolean gridlines_show_p;           /* scatterplot, scatmat, parcoords */
 gboolean axes_show_p;                /* scatterplot, scatmat, parcoords */
 gboolean axes_center_p;              /* scatterplot */

 gboolean double_buffer_p;            /* parcoords */

 gboolean link_p;                     /* scatterplot, scatmat, parcoords */
} DisplayOptions;

extern DisplayOptions DefaultDisplayOptions;



#define TYPES_H

#endif
