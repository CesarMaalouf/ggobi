#ifndef GGOBI_S_H
#define GGOBI_S_H

#include <stdio.h>
#ifndef DEFINES_H
#include "defines.h"
#endif

#include "barchartDisplay.h"

#ifdef __cplusplus
extern "C" {
#endif


RedrawStyle brush_activate (gboolean, displayd *, ggobid *);
RedrawStyle identify_activate (gint, displayd *, ggobid *);
RedrawStyle p1d_activate (gint, displayd *, ggobid *);
RedrawStyle viewmode_activate (splotd *, PipelineMode, gboolean, ggobid *);
RedrawStyle xyplot_activate (gint, displayd *, ggobid *);

/* sort -t":" -k1.12 */
void       vartable_copy_var (gint jfrom, gint jto, datad *d);
gboolean   array_contains (gint* arr, gint n, gint el);
vartabled* vartable_element_new (datad *d);
void       transform_values_copy (gint jfrom, gint jto, datad *d);
GtkCList*  vartable_clist_get (ggobid *gg);
GtkWidget* CreateMenuCheck (GtkWidget *, gchar *, GtkSignalFunc, gpointer, gboolean, ggobid *);
GtkWidget* CreateMenuItem (GtkWidget *, gchar *, gchar *, gchar *, GtkWidget *, GtkAccelGroup *, GtkSignalFunc, gpointer, ggobid *) ;
ggobid*    GGobiFromDisplay (displayd *display);
ggobid*    GGobiFromSPlot (splotd *sp);
ggobid*    GGobiFromWidget (GtkWidget *w, gboolean);
ggobid*    GGobiFromWindow (GdkWindow *w);
GtkWidget* GGobi_addToolsMenuItem (gchar *label, ggobid *gg);
void       GGobi_widget_set (GtkWidget *, ggobid *gg, gboolean isWindow);
datad*     ValidateDatadRef (datad *d, ggobid *gg, gboolean fatal);
displayd*  ValidateDisplayRef(displayd *d, ggobid *gg, gboolean fatal);
ggobid*    ValidateGGobiRef (ggobid *gg, gboolean fatal);
xmlNodePtr XML_addVariable(xmlNodePtr node, gint j, datad *d);
gint       alloc_optimize0_p (optimize0_param *op, gint nrows, gint ncols, gint ndim);
gboolean   array_read (datad *, InputDescription *,  ggobid *);
void       arrayd_add_cols (array_d *, gint);
void       arrayd_add_rows (array_d *, gint);
void       arrayd_alloc (array_d *, gint, gint);
void       arrayd_alloc_zero (array_d *, gint, gint);
void       arrayd_copy (array_d *, array_d *);
void       arrayd_delete_cols (array_d *, gint, gint *);
void       arrayd_free (array_d *, gint, gint);
void       arrayd_init_null (array_d *);
void       arrayd_zero (array_d *);
void       arrayf_add_cols (array_f *, gint);
void       arrayf_add_rows (array_f *, gint);
void       arrayf_alloc (array_f *, gint, gint);
void       arrayf_alloc_zero (array_f *, gint, gint);
void       arrayf_copy (array_f *, array_f *);
void       arrayf_delete_cols (array_f *, gint, gint *);
void       arrayf_free (array_f *, gint, gint);
void       arrayf_init_null (array_f *);
void       arrayf_zero (array_f *);
void       arrayg_add_cols (array_g *, gint);
void       arrayg_add_rows (array_g *, gint);
void       arrayg_alloc (array_g *, gint, gint);
void       arrayg_alloc_zero (array_g *, gint, gint);
void       arrayg_delete_cols (array_g *, gint, gint *);
void       arrayg_free (array_g *, gint, gint);
void       arrayg_init_null (array_g *);
void       arrayg_zero (array_g *);
void       arrayl_add_cols (array_l *, gint);
void       arrayl_add_rows (array_l *, gint);
void       arrayl_alloc (array_l *, gint, gint);
void       arrayl_alloc_zero (array_l *, gint, gint);
void       arrayl_delete_cols (array_l *, gint, gint *);
void       arrayl_free (array_l *, gint, gint);
void       arrayl_init_null (array_l *);
void       arrayl_zero (array_l *);
void       arrays_add_cols (array_s *, gint);
void       arrays_add_rows (array_s *, gint);
void       arrays_alloc (array_s *, gint, gint);
void       arrays_alloc_zero (array_s *, gint, gint);
void       arrays_delete_cols (array_s *, gint, gint *);
void       arrays_free (array_s *, gint, gint);
void       arrays_init_null (array_s *);
void       arrays_zero (array_s *);
void       ash_baseline_set (icoords *, splotd *sp);
void       assign_points_to_bins (datad *, ggobid *);
void       br_color_ids_alloc (datad *, ggobid *);
void       br_color_ids_init (datad *, ggobid *);
void       br_edge_color_alloc (ggobid *);
void       br_edge_color_init (datad *d, ggobid *);
gboolean   br_edge_vectors_check_size (gint, datad *, ggobid *);
void       br_glyph_ids_alloc (datad *);
void       br_glyph_ids_init (datad *, ggobid *);
void       brush_draw_brush (splotd *, GdkDrawable *, datad *, ggobid *);
void       brush_draw_label (splotd *, GdkDrawable *, datad *, ggobid *);
void       brush_event_handlers_toggle (splotd *, gboolean);
void       brush_init (datad *, ggobid *);
void       brush_motion (icoords *, gboolean, gboolean, cpaneld *, splotd *, ggobid *);
gboolean   brush_once (gboolean force, splotd *, ggobid *);
void       brush_once_and_redraw (gboolean binningp, splotd *sp, displayd *display, ggobid *gg);
void       brush_options_cb (gpointer, guint, GtkCheckMenuItem *);
void       brush_pos_init (splotd *);
void       brush_prev_vectors_update (datad *, ggobid *);
void       brush_reset(ggobid *gg, gint action);
void       brush_reset_cb (GtkWidget *w, gpointer cbd);
void       brush_set_pos (gint, gint, splotd *);
void       brush_undo (splotd *, datad *, ggobid *);
void       brush_update_set_cb (GtkCheckMenuItem *w, guint action);
gboolean   build_symbol_vectors_by_var (cpaneld *, datad *, ggobid *);
gdouble    calc_norm (gdouble *, gint);
gboolean   checkequiv(gdouble **u0, gdouble **u1, gint nc, gint nd);
gint       checkLevelValue(vartabled *vt, gdouble value);
void       clone_vars (gint *cols, gint ncols, datad *, ggobid *);
void       cluster_free (gint, datad *, ggobid *);
void       cluster_table_labels_update (datad *d, ggobid *gg);
void       cluster_table_update (datad *, ggobid *);
void       cluster_window_open (ggobid *);
void       clusters_set (datad *, ggobid *);
void       collab_tform_update (gint j, datad *d, ggobid *gg);
gboolean   collabels_read (InputDescription *desc, gboolean, datad *, ggobid *);
gboolean   colors_remap (colorschemed *scheme, gboolean force, ggobid *gg);
void       colorscheme_init (colorschemed *scheme);
gchar*     computeTitle (gboolean, displayd *, ggobid *);
void       copy_mat(gdouble **, gdouble **, gint, gint);
void       cpanel_brush_init (cpaneld *, ggobid *);
void       cpanel_brush_make (ggobid *);
void       cpanel_brush_set (cpaneld *, ggobid *);
void       cpanel_ctour_make (ggobid *);
void       cpanel_edgeedit_init (cpaneld *, ggobid *);
void       cpanel_edgeedit_make (ggobid *);
void       cpanel_edgeedit_set (cpaneld *, ggobid *);
void       cpanel_identify_init (cpaneld *, ggobid *);
void       cpanel_identify_make (ggobid *);
void       cpanel_identify_set (cpaneld *, ggobid *);
void       cpanel_movepts_make (ggobid *);
void       cpanel_p1d_init (cpaneld *, ggobid *);
void       cpanel_p1d_set (cpaneld *, ggobid *);
void       cpanel_p1dplot_make (ggobid *);
void       cpanel_parcoords_init (cpaneld *, ggobid *);
GtkWidget *cpanel_parcoords_make (ggobid *);
void       cpanel_parcoords_set (cpaneld *, GtkWidget *panel, ggobid *);
void       cpanel_rotation_init (cpaneld *, ggobid *);
void       cpanel_rotation_make (ggobid *);
void       cpanel_rotation_set (cpaneld *, ggobid *);
void       cpanel_scale_init (cpaneld *, ggobid *);
void       cpanel_scale_make (ggobid *);
void       cpanel_scale_set (cpaneld *, ggobid *);
void       cpanel_scatmat_init (cpaneld *, ggobid *);
void       cpanel_scatmat_make (ggobid *);
void       cpanel_scatmat_set (cpaneld *, ggobid *);
void       cpanel_set (displayd *, ggobid *);
void       cpanel_show_cb (GtkCheckMenuItem *w, guint action);
void       cpanel_t1d_init (cpaneld *, ggobid *);
void       cpanel_t2d_init (cpaneld *, ggobid *);
void       cpanel_t2d3_init (cpaneld *, ggobid *);
void       cpanel_tcorr_init (cpaneld *, ggobid *);
void       cpanel_tour1d_make (ggobid *);
void       cpanel_tour1d_set (cpaneld *, ggobid *);
void       cpanel_tour2d_make (ggobid *);
void       cpanel_tour2d_set (cpaneld *, ggobid *);
void       cpanel_tour2d3_make (ggobid *);
void       cpanel_tour2d3_set (cpaneld *, ggobid *);
void       cpanel_tourcorr_set (cpaneld *, ggobid *);
void       cpanel_xyplot_init (cpaneld *, ggobid *);
void       cpanel_xyplot_make (ggobid *);
void       cpanel_xyplot_set (cpaneld *, ggobid *);
displayd * createDisplayFromDescription (ggobid *, GGobiDisplayDescription *desc);
GtkWidget* create_variable_notebook (GtkWidget *box, GtkSelectionMode mode, vartyped vtype, datatyped dtype, GtkSignalFunc func, ggobid *);
void       ctour_event_handlers_toggle (splotd *, gboolean);
void       ctourpp_window_open (ggobid *);
colorschemed* default_scheme_init ();
gushort    datad_colors_used_get (gint *ncolors_used, gushort *colors_used, datad *, ggobid *); 
void       datad_free (datad *, ggobid *);
datad*     datad_get_from_notebook (GtkWidget *notebook, ggobid *);
gboolean   datad_has_edges (datad *d);
gboolean   datad_has_variables (datad *d);
displayd*  datad_init (datad *, ggobid *, gboolean);
datad*     datad_new(datad *d, ggobid *gg);
gchar*     datasetName (datad *d, ggobid *gg);
gint       delete_vars (gint *, gint, datad *, ggobid *);
void       disconnect_button_press_signal (splotd *sp);
void       disconnect_button_release_signal (splotd *sp);
void       disconnect_key_press_signal (splotd *sp);
void       disconnect_motion_signal (splotd *sp);
gint       display_add(displayd *display, ggobid *);
displayd*  display_alloc_init (gboolean, datad *, ggobid *);
void       display_close_cb (displayd *d, guint, GtkWidget *);
gboolean   display_copy_edge_options (displayd *dsp, displayd *dspnew);
displayd*  display_create (gint displaytype, gboolean missing_p, datad *, ggobid *);
void       display_delete_cb (GtkWidget *, GdkEvent *, displayd *);
void       display_free (displayd *, gboolean force, ggobid *);
void       display_free_all (ggobid *);
void       display_menu_build (ggobid *);
void       display_menu_init (ggobid *);
void       display_new (ggobid *, guint action, GtkWidget *widget);
void       display_options_cb (GtkCheckMenuItem *w, guint action);
void       display_plot (displayd *display, RedrawStyle type, ggobid *);
void       display_print_cb (displayd *d, guint, GtkWidget *);
void       display_set_current (displayd *, ggobid *);
void       display_set_position (windowDisplayd *d, ggobid *gg);
void       display_t1d_init (displayd *, ggobid *);
void       display_t2d_init (displayd *, ggobid *);
void       display_tailpipe (displayd *, RedrawStyle, ggobid *);
void       display_tour1d_init (displayd *dsp, ggobid *gg);
void       display_tour1d_init_null (displayd *dsp, ggobid *gg);
void       display_tour2d_init (displayd *dsp, ggobid *gg);
void       display_tour2d_init_null (displayd *dsp, ggobid *gg);
void       display_tour2d3_init (displayd *, ggobid *);
void       display_tour2d3_init_null (displayd *, ggobid *);
void       display_tourcorr_init (displayd *dsp, ggobid *gg);
void       display_tourcorr_init_null (displayd *dsp, ggobid *gg);
gboolean   display_type_handles_action (displayd *, PipelineMode m);
void       display_window_init (windowDisplayd *, gint, ggobid *);
void       display_write_svg (ggobid *);
void       displays_plot (splotd *, RedrawStyle, ggobid *);
void       displays_tailpipe (RedrawStyle, ggobid *);
gint       do_ash1d (gfloat *, gint, gint, gint, gfloat *, gfloat *, gfloat *, gfloat *);
void       do_last_increment (vector_f, vector_f, gfloat, gint);
void       draw_glyph (GdkDrawable *, glyphd *, icoords *, gint, ggobid *);
gint       dsvd (gdouble **a, gint m, gint n, gfloat *w, gdouble **v);
PipelineMode projection_get (ggobid *);
PipelineMode viewmode_get (ggobid *);
void       edgeedit_event_handlers_toggle (splotd *, gboolean);
void       edges_alloc (gint, datad *);
void       edges_create (datad *, ggobid *);
void       edges_free (datad *, ggobid *);
gboolean   edges_read (InputDescription *desc, gboolean, datad *, ggobid *);
gboolean   edgeset_add (displayd *);
void       edgeset_add_cb (GtkWidget *w, datad *e);
void       eigenvals_get (gfloat *, datad *);
gint       fcompare (const void *x1, const void *x2);
void       filename_get_r (ggobid *, guint, GtkWidget *);
void       filename_get_w (GtkWidget *, ggobid *);
gboolean   fileset_read (const gchar *, ggobid *);
gboolean   fileset_read_init (const gchar *ldata_in, ggobid *);
void       find_glyph_type_and_size (gint, glyphd *);
gint       find_keepers (gint ncols_current, gint nc, gint *cols, gint *keepers);
gint       find_nearest_point (icoords *, splotd *, datad *, ggobid *);
GList*     g_list_remove_nth (GList *, gint);
GList*     g_list_replace_nth (GList *, gpointer, gint);
gint       getPreviousDisplays(xmlNodePtr node, GGobiDescription *desc);
GtkWidget* get_clist_from_object (GtkObject *);
void       get_extended_brush_corners (icoords *, icoords *, datad *, splotd *);
gint       get_one_selection_from_clist (GtkWidget *clist, datad *);
gint       get_selections_from_clist (gint maxnvars, gint *vars, GtkWidget *clist, datad *);
ggobid*    ggobi_alloc (ggobid *tmp);
gboolean   ggobi_file_set_create (gchar *rootname, datad *, ggobid *);
ggobid*    ggobi_get (gint);
gint       ggobi_getIndex(ggobid *gg);
void       globals_init (ggobid *);
gboolean   gram_schmidt (gdouble *, gdouble*, gint);
void       gt_basis (array_d, gint, vector_i, gint, gint);
void       hidden_alloc (datad *);
void       hidden_init (datad *, ggobid *);
gboolean   hidden_read (InputDescription *desc, gboolean, datad *, ggobid *);
void       identify_event_handlers_toggle (splotd *, gboolean);
void       identify_link_by_id (gint k, datad *source_d, ggobid *gg);
gboolean   impute_fixed (gint, gint nvars, gint *vars, datad *, ggobid *);
void       impute_random (datad *, gint nvars, gint *vars, ggobid *);
void       impute_window_open (ggobid *);
void       increment_tour(vector_f, vector_f, gfloat, gfloat, gfloat *, gint);
void       initSessionOptions(void);
void       init_plot_GC (GdkWindow *, ggobid *);
void       init_var_GCs (GtkWidget *, ggobid *);
gdouble    inner_prod (gdouble *, gdouble *, gint);
gboolean   isCrossed (gdouble, gdouble, gdouble, gdouble, gdouble, gdouble, gdouble, gdouble);
gfloat     jitter_randval (gint);
void       jitter_value_set (gfloat, datad *, ggobid *);
void       jitter_vars_init (datad *, ggobid *);
void       jitter_window_open (ggobid *);
void       limits_adjust (gfloat *, gfloat *);
void       limits_set (gboolean do_raw, gboolean do_tform, datad *, ggobid *);  
void       limits_set_by_var (gint, gboolean do_raw, gboolean do_tform, datad *, ggobid *);  
gint       lines_intersect (glong, glong, glong, glong, glong, glong, glong, glong);
void       linking_method_set (displayd *, datad *, ggobid *);
void       make_ggobi (GGobiOptions *, gboolean, ggobid *);
void       make_symbol_window (ggobid *);
void       make_ui (ggobid *);
GlyphType  mapGlyphName (const gchar *gtype);
void       missing_arrays_add_cols (datad *d, ggobid *);
gboolean   missing_values_read (InputDescription *desc, gboolean, datad *, ggobid *);
void       missings_datad_cb (GtkWidget *w, ggobid *gg);
gboolean   mouseinwindow (splotd *);
void       mousepos_get_motion (GtkWidget *, GdkEventMotion *, gboolean *, gboolean *, splotd *);
void       mousepos_get_pressed (GtkWidget *, GdkEventButton *, gboolean *, gboolean *, splotd *);
void       move_pt (gint id, gint x, gint y, splotd *sp, datad *d, ggobid *);
void       movepts_event_handlers_toggle (splotd *, gboolean);
void       movepts_history_add (gint id, splotd *sp, datad *, ggobid *);
void       movepts_history_delete_last (datad *, ggobid *);
gdouble    myrint (gdouble x);
gint       ndatad_with_vars_get (ggobid *gg);
void       newvar_add (gint vtype, gchar *vname, datad *, ggobid *);
void       newvar_add (gint vtype, gchar *vname, datad *d, ggobid *gg);
void       newvar_add_with_values (gdouble *, gint, gchar *, datad *, ggobid *);
void       norm (gdouble *, gint);
gint       option_menu_index (GtkOptionMenu *);
GtkItemFactory *get_main_menu (const GtkItemFactoryEntry[], gint, GtkAccelGroup *, GtkWidget  *, GtkWidget **, gpointer);
void       p1d_event_handlers_toggle (splotd *, gboolean);
void       p1d_reproject (splotd *, greal **, datad *, ggobid *);
gboolean   p1d_varsel (splotd *, gint, gint *, gint, gint);
gint       p1dcycle_func (ggobid *gg);
void       pan_by_drag (splotd *, ggobid *);
void       pan_step (splotd *, gint, ggobid *);
void       parcoords_cpanel_init (cpaneld*, ggobid *);
void       parcoords_mode_menu_make (GtkAccelGroup *, GtkSignalFunc, ggobid *, gboolean);
displayd*  parcoords_new (gboolean, gint, gint *, datad *, ggobid *);
void       parcoords_reset_arrangement (displayd *, gint, ggobid *);
gboolean   parcoords_varsel (cpaneld *, splotd *, gint, gint *, ggobid *);
gint       parse_command_line (gint *argc, gchar **av);
gint       tour_path (array_d, array_d, array_d, gint, gint, array_d, array_d, array_d, vector_f, array_d, array_d, array_d, vector_f, vector_f, gfloat *, gfloat *);
gboolean   pca_calc (datad *, ggobid *);
void       pca_diagnostics_set (datad *d, ggobid *);
gint       pcompare (const void *, const void *);
void       pipeline_arrays_add_column (gint, datad *, ggobid *);
void       pipeline_arrays_alloc (datad *, ggobid *);
void       pipeline_arrays_free (datad *d, ggobid *);
void       pipeline_init (datad *, ggobid *);
gint       plotted_cols_get (gint *, datad *, ggobid *);
gboolean   point_colors_read (InputDescription *desc, gboolean, datad *, ggobid *);
gboolean   point_glyphs_read (InputDescription *desc, gboolean, datad *, ggobid *);
gboolean   point_in_which_bin (gint, gint, gint *, gint *, datad *, splotd *);
void       populate_option_menu (GtkWidget *, gchar **, gint, GtkSignalFunc, gchar *key, gpointer);
void       position_popup_menu (GtkMenu *menu, gint *px, gint *py, gpointer);
gboolean   processRestoreFile(const gchar * const fileName, ggobid *gg);
void       procs_activate (gboolean state, displayd *display, ggobid *gg);
void       quick_message (const gchar * const, gboolean);
void       quit_ggobi(ggobid *gg, gint action, GtkWidget *w);
gdouble    randvalue (void);
void       range_unset (ggobid *gg);
void       raw_to_tform_copy (void);
gboolean   reached_target(gfloat, gfloat, gint, gfloat *, gfloat *);
gboolean   read_ascii_data(InputDescription *desc, ggobid *gg);
gint       realloc_optimize0_p(optimize0_param *, gint, vector_i);
void       recenter_data (gint, datad *, ggobid *);
void       reinit_transient_brushing (displayd *, ggobid *);
void       rejitter (gint *, gint, datad *, ggobid *);
  /*void       reset_pp(displayd *);*/
void       reset_pp(datad *, gint, gint, ggobid *, void *);
void       rnorm2 (gdouble *, gdouble *);
void       rotation_event_handlers_toggle (splotd *, gboolean);
void       rowids_alloc (datad *);
void       rowids_init_null (datad *d);
void       rowidv_init (datad *d);
void       rowlabels_alloc (datad *d, ggobid *) ;
void       rowlabels_free (datad *d, ggobid *);
gboolean   rowlabels_read (InputDescription *desc, gboolean, datad *, ggobid *);
void       rows_in_plot_set (datad *d, ggobid *);
void       ruler_ranges_set (gboolean force, displayd *, splotd *, ggobid *);
void       scale_click_init (splotd *sp, ggobid *gg);
void       scale_event_handlers_toggle (splotd *, gboolean);
void       scale_set_default_values (GtkScale *scale);
void       scaling_visual_cues_draw (splotd *, GdkDrawable *, ggobid *);
void       scatmat_cpanel_init (cpaneld *, ggobid *);
void       scatmat_mode_menu_make (GtkAccelGroup *, GtkSignalFunc, ggobid *gg, gboolean);
displayd*  scatmat_new (gboolean, gint, gint *, gint, gint *, datad *, ggobid *);
gboolean   scatmat_varsel (GtkWidget *, cpaneld *, splotd *, gint, gint, gint *, gint, gboolean, ggobid *);
gboolean   scatmat_varsel_simple (cpaneld *, splotd *, gint, gint *, ggobid *);
void       scatterplot_cpanel_init (cpaneld *, PipelineMode, ggobid *);
void       scatterplot_display_edge_menu_update (displayd *, GtkAccelGroup *accel_group, GtkSignalFunc func, ggobid *gg);
void       scatterplot_display_menus_make (displayd *display, GtkAccelGroup *, GtkSignalFunc, ggobid *);
void       scatterplot_mode_menu_make (GtkAccelGroup *, GtkSignalFunc, ggobid *gg, gboolean);
displayd*  scatterplot_new (gboolean, splotd *sp, datad *d, ggobid *);
void       scatterplot_show_hrule (displayd *, gboolean show);
void       scatterplot_show_rulers (displayd *, gint);
void       scatterplot_show_vrule (displayd *, gboolean show);
gboolean   scree_mapped_p (ggobid *);
void       scree_plot_make (ggobid *);
gint       selected_cols_get (gint *, datad *d, ggobid *);
void       showInputDescription(InputDescription *desc, ggobid *gg);
void       smooth_window_open (ggobid *);
void       sp_event_handlers_toggle (splotd *, gboolean);
void       sp_whiskers_make (splotd *, displayd *, ggobid *);
void       speed_set (gfloat, gfloat *, gfloat *);
void       sphere_condnum_set (gfloat x, ggobid *);
void       sphere_enable (gboolean sens, ggobid *);
void       sphere_free (datad *);
void       sphere_init (datad *);
gint       sphere_npcs_get (datad *, ggobid *);
void       sphere_npcs_range_set (gint n, ggobid *gg);
void       sphere_npcs_set (gint, datad *, ggobid *);
void       sphere_panel_open (ggobid *);
void       sphere_varcovar_set (datad *, ggobid *);
void       sphere_variance_set (gfloat x, datad *, ggobid*);
void       spherevars_set (ggobid *);
void       spherize_data (vector_i *svars, vector_i *pcvars, datad *, ggobid *);
gboolean   spherize_set_pcvars (datad *, ggobid *);
void       splash_show (ggobid *gg, guint action, GtkWidget *w);
void       splot_cursor_set (gint jcursor, splotd *sp);
void       splot_dimension_set(splotd* sp, gint width, gint height);
void       splot_edges_realloc (splotd *, datad *, ggobid *);
gboolean   splot_event_handled (GtkWidget *, GdkEventKey *, cpaneld *, splotd *, ggobid *);
void       splot_expose (splotd *);
void       splot_free (splotd *, displayd *, ggobid *);
void       splot_get_dimensions (splotd *, gint *, gint *);
splotd*    splot_new (displayd *, gint, gint, ggobid *);
void       splot_pixmap0_to_pixmap1 (splotd *, gboolean, ggobid *);
void       splot_pixmap1_to_window (splotd *, ggobid *);
void       splot_plane_to_screen (displayd *, cpaneld *, splotd *, ggobid *);
void       splot_plane_to_world (splotd *, gint, ggobid *);
gboolean   splot_plot_case (gint m, gboolean, datad *, splotd *, displayd *, ggobid *);
void       splot_redraw (splotd *sp, RedrawStyle, ggobid *);
void       splot_reverse_pipeline (splotd *, gint, gcoords *, gboolean, gboolean, ggobid *);
void       splot_screen_to_tform (cpaneld *, splotd *, icoords *, fcoords *, ggobid *);
void       splot_set_current (splotd *, gboolean, ggobid *);
void       splot_set_plot_center (splotd *);
void       splot_world_to_plane (cpaneld *, splotd *, ggobid *);
void       splot_zoom (splotd *sp, gfloat xsc, gfloat ysc, ggobid *) ;
gint       sqdist (gint, gint, gint, gint);
void       sticky_id_toggle (datad *, ggobid *);
gint       strToInteger (const gchar *tmp);
void       strip_suffixes (ggobid *);
void       submenu_append (GtkWidget *, GtkWidget *);
void       submenu_destroy (GtkWidget *);
void       submenu_insert (GtkWidget *, GtkWidget *, gint);
GtkWidget* submenu_make (gchar *, guint, GtkAccelGroup *);
void       subset_apply (datad *, ggobid *);
gboolean   subset_block (gint, gint, datad *, ggobid *);
gboolean   subset_everyn (gint, gint, datad *, ggobid *);
void       subset_include_all (datad *, ggobid *);
gboolean   subset_random (gint, datad *, ggobid *);
gboolean   subset_rowlab (gchar *, datad *, ggobid *);
gboolean   subset_sticky (datad *, ggobid *);
void       subset_window_open (ggobid *, guint, GtkWidget *);
void       symbol_link_by_id (gint k, datad *source_d, ggobid *gg);
gint       symbol_table_populate (datad *d);
void       symbol_window_redraw (ggobid *);
void       t1d_clear_ppda (displayd *, ggobid *);
void       t1d_optimz (gint, gboolean *, gint *, displayd *);
void       t1d_pp_reinit(displayd *, ggobid *);
void       t1d_ppdraw(gfloat, displayd *, ggobid *);
void       t1d_pptemp_set(gfloat, displayd *, ggobid *);
void       t1d_ppcool_set(gfloat, displayd *, ggobid *);
void       t2d_clear_ppda (displayd *, ggobid *);
void       t2d_optimz (gint, gboolean *, gint *, displayd *);
void       t2d_pp_reinit(displayd *, ggobid *);
void       t2d_ppdraw (gfloat, displayd *, ggobid *);
void       t2d_pptemp_set(gfloat, displayd *, ggobid *);
void       t2d_ppcool_set(gfloat, displayd *, ggobid *);
void       textur (gfloat *, gfloat *, gint, gint, gfloat, gint, ggobid *);
void       tform1_to_tform2_copy (void);
void       tform_label_update (gint, datad *, ggobid *);
void       tform_to_world (datad *, ggobid *);
void       tform_to_world_by_var (gint j, datad *, ggobid *);
void       tooltips_show_cb (GtkCheckMenuItem *w, guint action);
void       tour1d_do_step (displayd *,ggobid *);
void       tour1d_event_handlers_toggle (splotd *, gboolean);
void       tour1d_fade_vars_cb (GtkCheckMenuItem *w, guint action);
void       tour1d_all_vars_cb (GtkCheckMenuItem *w, guint action);
void       tour1d_func (gboolean, displayd *, ggobid *);
void       tour1d_io_cb (GtkWidget *w, gpointer *cbd);
void       tour1d_manip (gint, gint, splotd *, ggobid *);
void       tour1d_manip_end (splotd *);
void       tour1d_manip_init (gint, gint, splotd *);
void       tour1d_pause (cpaneld *, gboolean, displayd *, ggobid *);
void       tour1d_projdata (splotd *, greal **, datad *, ggobid *);
void       tour1d_realloc_down (gint nc, gint *cols, datad *d, ggobid *gg);
void       tour1d_realloc_up (gint nc, datad *d, ggobid *gg);
void       tour1d_reinit (ggobid *);
void       tour1d_scramble(ggobid *);
void       tour1d_speed_set (gfloat, ggobid *);
gboolean   tour1d_varsel (GtkWidget *, gint jvar, gint toggle, gint btn, datad *, ggobid *);
void       tour1d_vert (cpaneld *, gboolean);
void       tour1dpp_window_open (ggobid *);
void       tour2d_do_step (displayd *,ggobid *);
void       tour2d_event_handlers_toggle (splotd *, gboolean);
void       tour2d_fade_vars_cb (GtkCheckMenuItem *w, guint action);
void       tour2d_all_vars_cb (GtkCheckMenuItem *w, guint action);
void       tour2d_func (gboolean, displayd *, ggobid *);
void       tour2d_io_cb (GtkWidget *w, gpointer *cbd);
void       tour2d_manip (gint, gint, splotd *, ggobid *);
void       tour2d_manip_end (splotd *);
void       tour2d_manip_init (gint, gint, splotd *);
void       tour2d_pause (cpaneld *, gboolean, displayd *, ggobid *);
void       tour2d_projdata (splotd *, greal **, datad *, ggobid *);
void       tour2d_realloc_down (gint nc, gint *cols, datad *d, ggobid *gg);
void       tour2d_realloc_up (gint nc, datad *d, ggobid *gg);
void       tour2d_reinit (ggobid *);
void       tour2d_scramble (ggobid *);
void       tour2d_speed_set (gfloat, ggobid *);
gboolean   tour2d_varsel (GtkWidget *, gint jvar, gint toggle, gint btn, datad *, ggobid *);
void       tour2dpp_window_open (ggobid *);
void       tour2d3_event_handlers_toggle (splotd *, gboolean state);
void       tour2d3_func (gboolean state, displayd *, ggobid *);
void       tour2d3_manip (gint, gint, splotd *, ggobid *);
void       tour2d3_manip_end (splotd *);
void       tour2d3_manip_init (gint, gint, splotd *);
void       tour2d3_menus_make (ggobid *);
void       tour2d3_pause (cpaneld *, gint, ggobid *);
void       tour2d3_projdata (splotd *, greal **world_data, datad *, ggobid *);
void       tour2d3_realloc_up (gint nc, datad *d, ggobid *gg);
void       tour2d3_reinit (ggobid *);
void       tour2d3_scramble (ggobid *);
void       tour2d3_speed_set (gfloat, ggobid *);
gboolean   tour2d3_varsel (GtkWidget *, gint jvar, gint toggle, gint btn, datad *, ggobid *);
void       tour_do_step (displayd *, ggobid *);
void       tour_manip_colors_init (ggobid *);
void       tour_reproject (vector_f, array_d, array_d, array_d, array_d, array_d, gint, gint);
void       tourcorr_fade_vars_cb (GtkCheckMenuItem *w, guint action);
void       tourcorr_func (gboolean, displayd *, ggobid *);
void       tourcorr_io_cb (GtkWidget *w, gpointer *cbd);
void       tourcorr_manip (gint, gint, splotd *, ggobid *);
void       tourcorr_manip_end (splotd *);
void       tourcorr_manip_init (gint, gint, splotd *);
void       tourcorr_pause (cpaneld *, gboolean, ggobid *);
void       tourcorr_projdata (splotd *, greal **, datad *, ggobid *);
void       tourcorr_realloc_down (gint nc, gint *cols, datad *d, ggobid *gg);
void       tourcorr_realloc_up (gint nc, datad *d, ggobid *gg);
void       tourcorr_reinit (ggobid *);
void       tourcorr_scramble (ggobid *);
void       tourcorr_speed_set (gfloat, ggobid *);
gboolean   tourcorr_varsel (GtkWidget *, gint jvar, gint toggle, gint btn, datad *, ggobid *);
void       transform (gint, gint, gfloat, gint *, gint, datad *, ggobid *);
void       transform0_opt_menu_set_value (gint j, datad *d, ggobid *gg);
void       transform0_values_set (gint, gint, datad *, ggobid *);
gboolean   transform1_apply (gint, datad *, ggobid *);
void       transform1_opt_menu_set_value (gint j, datad *d, ggobid *gg);
void       transform1_values_set (gint, gfloat, gint, datad *, ggobid *);
gboolean   transform2_apply (gint, datad *, ggobid *);
void       transform2_opt_menu_set_value (gint j, datad *d, ggobid *gg);
void       transform2_values_set (gint, gint, datad *, ggobid *);
void       transform_values_init (gint j, datad *, ggobid *);
void       transform_variable (gint, gint, gfloat, gint, datad *, ggobid *);
void       transform_window_open (ggobid *);
gboolean   update_color_vectors (gint i, gboolean changed, gboolean *hit_by_brush, datad *d, ggobid *gg);
gboolean   update_glyph_vectors (gint i, gboolean changed, gboolean *hit_by_brush, datad *d, ggobid *gg);
void       varcircle_label_set (gint jvar, datad *d);
void       varcircle_label_set (gint, datad *);
void       varcircles_add (gint ncols, datad *, ggobid *);
void       varcircles_cursor_set_default (datad *d);
void       varcircles_delete_nth (gint j, datad *);
void       varcircles_populate (datad *, ggobid *);
void       varcircles_refresh (datad *, ggobid *);
void       varcircles_show (gboolean, datad *, displayd *, ggobid *);
void       varcircles_visibility_set (displayd *display, ggobid *gg);
void       vardialog_open (ggobid *, gchar *title);
void       variable_clone (gint, const gchar *, gboolean, datad *, ggobid *);
void       variable_notebook_list_changed_cb(ggobid *gg, datad *d, void *notebook);
void       variable_notebook_subwindow_add (datad *d, GtkSignalFunc func, GtkWidget *notebook, vartyped, datatyped, ggobid *gg);
void       variable_notebook_varchange_cb (ggobid *gg, vartabled *vt, gint which, datad *, void *notebook);
void       varpanel_clear (datad *, ggobid *);
void       varpanel_delete_nth (gint jvar, datad *d);
void       varpanel_label_set (gint, datad *);
void       varpanel_make (GtkWidget *, ggobid *);
void       varpanel_populate (datad *, ggobid *);
void       varpanel_refresh (displayd *, ggobid *);
void       varpanel_reinit (ggobid *gg);
void       varpanel_show_page (displayd*, ggobid*);
void       varpanel_tooltips_set (displayd *, ggobid *);
GtkWidget* varpanel_widget_get_nth (gint jbutton, gint jvar, datad *d);
void       varpanel_widgets_add (gint nc, datad *d, ggobid *gg);
void       vars_stdized_send_event (datad *d, ggobid *gg);
void       varsel (GtkWidget *w, cpaneld *, splotd *, gint jvar, gint toggle, gint btn, gint alt_mod, gint ctrl_mod, gint shift_mod, datad *, ggobid *);
void       vartable_alloc (datad *);
void       vartable_cells_set_by_var (gint j, datad *d);
void       vartable_collab_set_by_var (gint, datad *);
void       vartable_collab_tform_set_by_var (gint j, datad *d);
vartabled *vartable_element_get (gint j, datad *d);
vartabled *vartable_element_get_by_name (gchar *collab, datad *d);
void       vartable_element_remove (gint, datad *);
gint       vartable_index_get_by_name (gchar *collab, datad *d);
void       vartable_init (datad *d);
void       vartable_limits_set (datad *);
void       vartable_limits_set_by_var (gint j, datad *d);
void       vartable_open (ggobid *);
void       vartable_realloc (gint, datad *);
void       vartable_row_append (gint j, datad *, ggobid *);
void       vartable_show_page (displayd*, ggobid*);
void       vartable_stats_set (datad *);
void       vartable_stats_set_by_var (gint j, datad *);
void       vectorb_alloc (vector_b *, gint);
void       vectorb_alloc_zero (vector_b *, gint);
void       vectorb_copy (vector_b *, vector_b *);
void       vectorb_delete_els (vector_b *vecp, gint nels, gint *els);
void       vectorb_free (vector_b *);
void       vectorb_init_null (vector_b *);
void       vectorb_realloc (vector_b *, gint);
void       vectorb_realloc_zero (vector_b *, gint);
void       vectorb_zero (vector_b *vecp);
void       vectorf_alloc (vector_f *, gint);
void       vectorf_alloc_zero (vector_f *, gint);
void       vectorf_delete_els (vector_f *vecp, gint nels, gint *els);
void       vectorf_free (vector_f *);
void       vectorf_init_null (vector_f *);
void       vectorf_realloc (vector_f *, gint);
void       vectorf_zero (vector_f *vecp);
void       vectorg_alloc (vector_g *, gint);
void       vectorg_copy (vector_g *, vector_g *);
void       vectorg_free (vector_g *);
void       vectorg_init_null (vector_g *);
void       vectorg_realloc (vector_g *, gint);
void       vectorg_realloc_zero (vector_g *, gint);
void       vectori_alloc (vector_i *, gint);
void       vectori_alloc_zero (vector_i *, gint);
void       vectori_copy (vector_i *, vector_i *);
void       vectori_delete_els (vector_i *vecp, gint nels, gint *els);
void       vectori_free (vector_i *);
void       vectori_init_null (vector_i *);
void       vectori_realloc (vector_i *, gint);
void       vectors_copy (vector_s *, vector_s *);
void       vectors_free (vector_s *);
void       vectors_init_null (vector_s *);
void       vectors_realloc (vector_s *, gint);
void       vectors_realloc_zero (vector_s *, gint);
void       viewmode_set (PipelineMode, ggobid *);
void       viewmode_set_cb (GtkWidget  *, gint);
void       viewmode_submenus_update (PipelineMode prev_mode, displayd *, ggobid *);
GtkWidget* widget_find_by_name (GtkWidget *, gchar *);
void       widget_initialize (GtkWidget *w, gboolean initd);
gboolean   widget_initialized (GtkWidget *w);
void       world_to_raw (gint, splotd *, datad *, ggobid *);
void       writeall_window_open (ggobid *);
void       wvis_init (ggobid *gg);
void       wvis_window_open (ggobid *gg);
void       xy_reproject (splotd *, greal **, datad *, ggobid *);
gint       xycycle_func (ggobid *gg);
void       xyplot_cycle_activate (gboolean state, cpaneld *cpanel, ggobid *gg);
void       xyplot_event_handlers_toggle (splotd *, gboolean);
gboolean   xyplot_varsel (splotd *, gint, gint *, gint, gint);
void       zoom_by_drag (splotd *, ggobid *);
void       zoom_step (splotd *, gint, gint, rectd *, ggobid*);


/*tsplot functions*/
GtkWidget* cpanel_tsplot_make (ggobid *);
void      cpanel_tsplot_set (cpaneld *, GtkWidget *, ggobid *);
void      tsplot_cpanel_init (cpaneld*, ggobid *);
void      tsplot_menus_make (ggobid *gg);
void      tsplot_mode_menu_make (GtkAccelGroup *, GtkSignalFunc, ggobid *gg, gboolean);
displayd* tsplot_new (gboolean, gint, gint *, datad *, ggobid *);
void      tsplot_reset_arrangement (displayd *, gint, ggobid *);
gboolean  tsplot_varsel (GtkWidget *, displayd *display, splotd *sp, gint jvar, gint toggle, gint mouse, cpaneld *cpanel,  ggobid *gg);
void      tsplot_whiskers_make (splotd *, displayd *, ggobid *);
void      zero_tau(vector_f, gint);

/* The new way of handling window closures, so that we don't just exit. */
gboolean  ggobi_close (ggobid *gg, GdkEvent *ev, GtkObject *w);
gint      ggobi_remove_by_index (ggobid *gg, gint which);
gint      ggobi_remove (ggobid *);
void      subset_init (datad *d, ggobid *gg);



void barchart_scaling_visual_cues_draw (splotd *sp, GdkDrawable *drawable, ggobid *gg);
gboolean barchart_active_paint_points (splotd *sp, datad *d); 
void barchart_add_bar_cues (splotd *sp, GdkDrawable *drawable, ggobid *gg);
void barchart_clean_init (barchartSPlotd *sp);
void barchart_cpanel_init (cpaneld *, ggobid *);
void barchart_event_handlers_toggle (splotd *, gboolean state);
void barchart_free_structure (barchartSPlotd *sp);
gboolean barchart_identify_bars (icoords mousepos, splotd *sp, datad *d, ggobid *gg);
void barchart_init_vectors(barchartSPlotd *sp);
GtkWidget *barchart_mode_menu_make (GtkAccelGroup *accel_group, GtkSignalFunc func, ggobid *gg, gboolean useIds);
displayd *barchart_new (gboolean missing_p, splotd *sp, datad *d, ggobid *gg);
void barchart_recalc_counts (barchartSPlotd *sp, datad *d, ggobid *gg);
void barchart_recalc_dimensions (splotd *sp, datad *d, ggobid *gg);
void barchart_recalc_group_dimensions (barchartSPlotd *sp, ggobid *gg);
gboolean barchart_redraw (splotd *sp, datad *d, ggobid *gg, gboolean binned);
void barchart_splot_add_plot_labels (splotd *, GdkDrawable *, ggobid *);
GtkWidget* cpanel_barchart_make (ggobid *gg);
void cpanel_barchart_set (cpaneld *cpanel, GtkWidget *panel, ggobid *gg);

void barchart_scale_event_handlers_toggle(splotd *sp, gboolean state);

displayd *barchart_new_with_vars(gboolean missing_p, gint nvars, gint *vars, datad * d, ggobid * gg);
#endif


#ifdef WIN32
void win32_draw_to_pixmap_binned (icoords *, icoords *, gint, splotd *, ggobid *gg);
void win32_draw_to_pixmap_unbinned (gint, splotd *, ggobid *gg);
#endif

#ifdef __cplusplus
}
#endif

#include "GGobiAPI.h"

#define CHECK_GG(a) ValidateGGobiRef(a, true)

/* Made externs for access from display class methods. */
void varpanel_toggle_set_active (gint jbutton, gint jvar, gboolean active, datad *d);
GtkWidget *varpanel_widget_set_visible (gint jbutton, gint jvar, gboolean show, datad *d);


void display_plot (displayd *display, RedrawStyle type, ggobid *gg);

gboolean array_contains (gint* arr, gint n, gint el);

void world_to_raw_by_var (gint pt, gint j, displayd *display, datad *d, ggobid *gg);
void splot_nearest_edge_highlight (splotd *sp, gint k, gboolean nearest, ggobid *gg);
void scatterXYAddPlotLabels(splotd *sp, GdkDrawable *drawable, GdkGC *gc);
void scatter1DAddPlotLabels(splotd *sp, GdkDrawable *drawable, GdkGC *gc);



gboolean processRestoreFile(const gchar * const fileName, ggobid *gg);

void scatterplotMovePointsMotionCb(displayd *display, splotd *sp, GtkWidget *w, GdkEventMotion *event, ggobid *gg);
void scatterplotMovePointsButtonCb(displayd *display, splotd *sp, GtkWidget *w, GdkEventButton *event, ggobid *gg);

displayd *scatterplot_new_with_vars(gboolean missing_p, gint numVars, gint *vars, datad *d, ggobid *gg);

gboolean array_contains (gint* arr, gint n, gint el);

void ggobiInit(int *argc, char **argv[]);

GGobiPluginInfo *readPluginFile(const char * const fileName, GGobiInitInfo *info);
gboolean registerPlugin(ggobid *gg, GGobiPluginInfo *plugin);
