#ifndef GGOBI_H
#define GGOBI_H

#include "defines.h"
#include "types.h"
#include "display.h"
#include "display_tree.h"

#include "varseldata.h"

typedef enum {ascii, Sprocess, binary, xml} DataMode;
typedef enum {read_all, read_block, draw_sample} FileReadType;

struct _ggobid;


typedef struct {
 gint nspherevars;
 gint *spherevars;
 gint sphere_npcs;

 gfloat *eigenval;
 gfloat **eigenvec;
 gfloat **vc;
 gfloat *tform1_mean;
} spherical;

typedef struct {

 struct _ggobid *thisGG;

  /* main_ui */
  GtkWidget *menubar;
  GtkAccelGroup *main_accel_group;

  /* brush_ui */
  GtkWidget *brush_reset_menu;
  GtkWidget *brush_link_menu;

  /* brush */
  brush_coords brush_pos ;  

  /* color_ui.c */
  gint spacing;

  /* identify_ui.c */
  GtkWidget *identify_link_menu;
  icoords cursor_pos;
  gint nearest_point, nearest_point_prev; 

  /* texture */
  gfloat *gy;

  /* tour2d_ui */
  GtkWidget *tour2d_io_menu;

  /* parcoords_ui */
  GtkWidget *parcoords_mode_menu;

  /* scatmat_ui */
  GtkWidget *scatmat_mode_menu;

  /* scatterplot_ui */
  GtkWidget *scatterplot_mode_menu;

  /* display_tree */
  DisplayTree display_tree;

  /* rotate_ui */
  GtkWidget *rotation_io_menu;

  /* scale_ui.c */
  GtkWidget *scale_reset_menu;

  varseldatad vdata0;
  varseldatad vdata1;

  GtkWidget *mode_item;
  gboolean firsttime;  


  /* xyplot_ui */
  gboolean cycle_p;
  gint direction;

  /* vartable_ui */
  GtkWidget *vardata_window;
  GtkWidget *clist;

  /* sp_plot.c */

/*
 * The corners of the bin to be copied from pixmap0 to pixmap1.
 * They're defined in splot_draw_to_pixmap0_binned and used in
 * splot_pixmap0_to_pixmap1 when binned == true.
*/
 icoords bin0, bin1;
 icoords loc0, loc1;

  /* sphere_ui */
 GtkWidget *window;
 GtkAdjustment *npcs_adj;
 GtkWidget *totvar_entry, *condnum_entry;
 GtkWidget *sphere_apply_btn;

  /* scatterplot.c */
 GtkAccelGroup *sp_accel_group;

  /* subset_ui*/
 gboolean rescale_p;
 GtkWidget *subset_window;
 GtkWidget *ss_notebook;

/*-- the entry widgets from which to get values for sample, rowlab --*/
 GtkWidget *ss_random_entry, *ss_rowlab_entry;
/*-- the adjustments from which to get values for blocksize, everyn --*/
 GtkAdjustment *ss_bstart_adj, *ss_bsize_adj;
 GtkAdjustment *ss_estart_adj, *ss_estep_adj;

  /* ggobi.h */
 GtkWidget *stage0_opt, *stage1_opt, *stage2_opt;
 GtkAdjustment *boxcox_adj;

  /* main_ui.c */
 gint mode , prev_mode;
 gint projection, prev_projection;
 GtkWidget *mode_frame;





} GGobiApp;

struct _ggobid {

 gchar *data_in;

 GdkGC *plot_GC;
 GdkGC *selvarfg_GC, *selvarbg_GC;     /* white background, thick lines */
 GdkGC *unselvarfg_GC, *unselvarbg_GC; /* grey background, thin lines */

 GList *displays;
 displayd *current_display;
 splotd *current_splot; 

 icoords mousepos, mousepos_o;
 gboolean mono_p;

 GtkWidget *control_panel[NMODES];

/************************** Data variables *************************/

 DataMode data_mode;

 gchar *filename;      /* full file name, including path */
 gchar *fname;         /* file name without suffix: .dat, .missing */

 /*
  * Do we read in the entire file, or do we only read in some
  * block or sample of cases?
 */
 FileReadType file_read_type;
 glong file_start_row;     /* needed for block type */
 glong file_length;        /* needed for sample */
 glong file_sample_size;   /* needed for both */
 /*-- used in reading in associated row-wise files --*/
 glong *file_rows_sampled; /* of length file_sample_size */

 gint ncols, nrows;

 array_f raw, tform1, tform2;
 array_l world, jitter;

/* Missing values */
 gint nmissing;
 array_s missing;  /*-- array of shorts --*/
 array_l missing_jitter, missing_world;
 gfloat missing_jitter_factor;
 lims missing_lim;  /*-- usually 0,1, but potentially wider --*/
/* */

 /* Deleting the hidden points; subsetting */
 gboolean delete_hidden_pts;
 gint *rows_in_plot;
 gint nrows_in_plot;
 gboolean *sampled;

 /* Row grouping */

 glong nrgroups, nrgroups_in_plot;
 glong *rgroup_ids;
 rgroupd *rgroups;

 /* Hiding/excluding the hidden points */
 clusterd *clusv;
 gint nclust;
 vector_i clusterid;
 gboolean *included;


/* Line groups */
 glong nlgroups;
 glong *lgroup_ids;
 rgroupd *lgroups;  /* id, nels, *els */

/* standardization options */
 gint std_type;  /* Can be 0, 1 or 2 */

/* for connecting points with segments */
 gint nsegments;
 endpointsd *segment_endpoints;

 vardatad *vardata;

/* row labels */
 gchar **rowlab;

/* gboolean: does this data contain only one variable? False by default */
 gboolean single_column;

/*
 * brushing
*/
 gint ncolors;
 GdkColor *default_color_table, *color_table;  /* brushing colors */
 GdkColor bg_color;          /* background color */
 GdkColor accent_color;      /* color for axes and labels */
 gushort color_id, color_0;  /* 0:ncolors-1 */
 /* point brushing */
 gint npts_under_brush;
 gboolean *pts_under_brush;
 gushort *color_ids, *color_now, *color_prev;  /* 0:ncolors-1 */
 glyphv glyph_id, glyph_0;
 glyphv *glyph_ids, *glyph_now, *glyph_prev;
 gboolean *hidden, *hidden_now, *hidden_prev;
 /* line brushing */
 gint *nxed_by_brush;
 gboolean *xed_by_brush;
 gushort *line_color, *line_color_now, *line_color_prev;
 gushort *line_hidden, *line_hidden_now, *line_hidden_prev;
 /* binning */
 gint br_nbins;
 bin_struct **br_binarray;
 icoords bin0, bin1;

 /*
  * jittering
 */
 gfloat jitter_factor;
 gboolean jitter_type;
 gboolean jitter_vgroup;
 gboolean jitter_convex;
 gfloat *jitfacv;

 /*
  * identification
 */
 GSList *sticky_ids;

 gint ntourvars;
 gint *tourvars;

/************************** Display variables *************************/

  GtkTooltips *tips;

/*
 * scaling
*/
 gint pan_or_zoom;

 GGobiApp app;

 spherical sphere;


  struct _Ash {
     GtkWidget *type_opt;
     GtkObject *ash_smoothness_adj;
     GtkObject *cycle_speed_adj;
     gboolean cycle_p;
  } ash;

  struct _Parcoords {
    GtkAccelGroup *pc_accel_group;
    GtkWidget *arrangement_box;
  } parcoords;


  struct _Color_UI {
   GtkWidget *symbol_window;
   GtkWidget *symbol_display;

   GtkWidget *colorseldlg;
   GtkWidget *bg_da, *accent_da, *fg_da[NCOLORS], *current_da;

   gint margin;  /* between glyphs in the symbol_display */
  } color_ui;


  struct _Varpanel_ui {
    GtkWidget *varpanel;
    GtkWidget *scrolled_window;
    GtkWidget **da, **varlabel;
    GtkAccelGroup *varpanel_accel_group;
    
    gint vnrows, vncols;
  } varpanel_ui;


  struct {
   varseldatad vdata0, vdata1;
  } p1d_menu;

  struct {
   varseldatad vdata0, vdata1;
  } parcoords_menu;

  struct {
   varseldatad vdata0, vdata1;
  } xyplot_menu;

  struct {
   varseldatad vdata0, vdata1, vdata2;
  } rotation_menu;

  struct {
   varseldatad vdata0, vdata1, vdata2;
  } tour2d_menu;

  struct {
   varseldatad vdata0, vdata1, vdata2, vdata3;
  } scatmat_menu;


  gint tour_idled; 

}; /*  ggobid; */


/* 
  GGOBI_ ggobid gg;
 */
/* extern ggobid gg; */


#define GGOBI_H

#endif

