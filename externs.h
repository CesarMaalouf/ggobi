#ifndef GGOBI_S_H
#define GGOBI_S_H

#include <stdio.h>

#ifdef USE_CLASSES
datad *datad_new (ggobid *);
#endif

datad *datad_new(datad *d, ggobid *gg);

#ifdef __cplusplus
"C" {
#endif

/* sort +1 */
void display_tour2d_init (displayd *dsp, ggobid *gg);
void display_tour1d_init (displayd *dsp, ggobid *gg);
void display_tourcorr_init (displayd *dsp, ggobid *gg);
GtkWidget* CreateMenuCheck (displayd *, GtkWidget *, gchar *, GtkSignalFunc, gpointer, gboolean, ggobid *);
GtkWidget* CreateMenuItem (GtkWidget *, gchar *, gchar *, gchar *, GtkWidget *, GtkAccelGroup *, GtkSignalFunc, gpointer, ggobid *) ;
ggobid* GGobiFromDisplay (displayd *display);
ggobid* GGobiFromSPlot (splotd *sp);
ggobid* GGobiFromWidget (GtkWidget *w, gboolean);
ggobid* GGobiFromWindow (GdkWindow *w);
void GGobi_widget_set (GtkWidget *, ggobid *gg, gboolean isWindow);
datad * ValidateDatadRef(datad *d, ggobid *gg, gboolean fatal);
displayd *ValidateDisplayRef(displayd *d, ggobid *gg, gboolean fatal);
ggobid* ValidateGGobiRef (ggobid *gg, gboolean fatal);
gboolean array_read (datad *, InputDescription *,  ggobid *);
void arrayf_add_cols (array_f *, gint);
void arrayf_add_rows (array_f *, gint);
void arrayf_alloc (array_f *, gint, gint);
void arrayf_alloc_zero (array_f *, gint, gint);
void arrayf_copy (array_f *, array_f *);
void arrayf_delete_cols (array_f *, gint, gint *);
void arrayf_free (array_f *, gint, gint);
void arrayf_init (array_f *);
void arrayf_zero (array_f *);
void arrayl_add_cols (array_l *, gint);
void arrayl_add_rows (array_l *, gint);
void arrayl_alloc (array_l *, gint, gint);
void arrayl_alloc_zero (array_l *, gint, gint);
void arrayl_delete_cols (array_l *, gint, gint *);
void arrayl_free (array_l *, gint, gint);
void arrayl_init (array_l *);
void arrayl_zero (array_l *);
void arrays_add_cols (array_s *, gint);
void arrays_add_rows (array_s *, gint);
void arrays_alloc (array_s *, gint, gint);
void arrays_alloc_zero (array_s *, gint, gint);
void arrays_delete_cols (array_s *, gint, gint *);
void arrays_free (array_s *, gint, gint);
void arrays_init (array_s *);
void arrays_zero (array_s *);
void assign_points_to_bins (datad *, ggobid *);
void br_color_ids_alloc (datad *, ggobid *);
void br_color_ids_init (datad *, ggobid *);
void br_glyph_ids_alloc (datad *);
void br_glyph_ids_init (datad *, ggobid *);
void br_line_color_alloc (ggobid *);
void br_line_color_init (datad *d, ggobid *);
gboolean br_line_vectors_check_size (gint, datad *, ggobid *);
void brush_activate (gboolean, datad *d, ggobid *);
void brush_draw_brush (splotd *, datad *d, ggobid *);
void brush_draw_label (splotd *, datad *d, ggobid *);
void brush_event_handlers_toggle (splotd *, gboolean);
void brush_init (datad *, ggobid *);
void brush_menus_make (ggobid *);
void brush_motion (icoords *, gboolean, gboolean, cpaneld *, datad *, ggobid *);
gboolean brush_once (gboolean force, datad *d, ggobid *);
void brush_options_cb (gpointer, guint, GtkCheckMenuItem *);
void brush_pos_init (datad *);
void brush_reset(ggobid *gg, gint action);
void brush_set_pos (gint, gint, datad *, ggobid *);
void checkbox_delete_nth (gint jvar, datad *);
void clone_vars (gint *cols, gint ncols, datad *, ggobid *);
void cluster_free (gint, datad *, ggobid *);
void clusters_set (datad *, ggobid *);
gboolean collabels_read (InputDescription *desc, gboolean, datad *, ggobid *);
void color_table_init (ggobid *);
gchar* computeTitle (gboolean, displayd *, ggobid *);
void cpanel_brush_init (cpaneld *, ggobid *);
void cpanel_brush_make (ggobid *);
void cpanel_brush_set (cpaneld *, ggobid *);
void cpanel_ctour_make (ggobid *);
void cpanel_identify_make (ggobid *);
void cpanel_lineedit_make (ggobid *);
void cpanel_movepts_make (ggobid *);
void cpanel_p1d_init (cpaneld *, ggobid *);
void cpanel_p1d_set (cpaneld *, ggobid *);
void cpanel_p1dplot_make (ggobid *);
void cpanel_parcoords_init (cpaneld *, ggobid *);
void cpanel_parcoords_make (ggobid *);
void cpanel_rotation_init (cpaneld *, ggobid *);
void cpanel_rotation_make (ggobid *);
void cpanel_rotation_set (cpaneld *, ggobid *);
void cpanel_scale_init (cpaneld *, ggobid *);
void cpanel_scale_make (ggobid *);
void cpanel_scatmat_init (cpaneld *, ggobid *);
void cpanel_scatmat_make (ggobid *);
void cpanel_set (displayd *, ggobid *);
void cpanel_t2d_init (cpaneld *, ggobid *);
void cpanel_t1d_init (cpaneld *, ggobid *);
void cpanel_tour1d_make (ggobid *);
void cpanel_tour2d_make (ggobid *);
void cpanel_xyplot_make (ggobid *);
void ctour_event_handlers_toggle (splotd *, gboolean);
void ctourpp_window_open (ggobid *);
void datad_free (datad *, ggobid *);
datad* datad_get_from_notebook (GtkWidget *notebook, ggobid *);
displayd* datad_init (datad *, ggobid *, gboolean);
void delete_vars (gint *, gint, datad *, ggobid *);
gint display_add(displayd *display, ggobid *);
void display_close_cb (displayd *d, guint, GtkWidget *);
displayd* display_create (gint displaytype, gboolean missing_p, datad *, ggobid *);
void display_delete_cb (GtkWidget *, GdkEvent *, displayd *);
void display_free (displayd *, gboolean force, ggobid *);
void display_free_all (ggobid *);
void display_menu_build (ggobid *);
void display_menu_init (ggobid *);
void display_new (ggobid *, guint action, GtkWidget *widget);
void display_options_cb (GtkCheckMenuItem *w, guint action);
void display_plot (displayd *display, guint type, ggobid *);
void display_print_cb (displayd *d, guint, GtkWidget *);
void display_set_current (displayd *, ggobid *);
void display_set_position (displayd *d, ggobid *gg);
void display_tailpipe (displayd *, ggobid *);
void display_tailpipe (displayd *, ggobid *);
void display_t2d_init (displayd *, ggobid *);
void display_t1d_init (displayd *, ggobid *);
void display_window_init (displayd *, gint, ggobid *);
void displays_plot (splotd *, gint, ggobid *);
void displays_tailpipe (gint, ggobid *);
gint do_ash1d (gfloat *, gint, gint, gint, gfloat *, gfloat *, gfloat *, gfloat *);
void draw_glyph (GdkDrawable *, glyphv *, icoords *, gint, ggobid *);
gint dsvd (gfloat **a, gint m, gint n, gfloat *w, gfloat **v);
void edges_alloc (gint, datad *, ggobid *);
void edges_create (datad *, ggobid *);
void edges_free (datad *, ggobid *);
gboolean edges_read (InputDescription *desc, gboolean, datad *, ggobid *);
void eigenvals_get (gfloat *, datad *);
void exclusion_window_open (ggobid *);
gint fcompare (const void *x1, const void *x2);
void filename_get_r (ggobid *, guint, GtkWidget *);
void filename_get_w (GtkWidget *, ggobid *);
gboolean fileset_read (const gchar *, DataMode mode, ggobid *);
gboolean fileset_read_init (const gchar *ldata_in, DataMode mode, ggobid *);
void find_glyph_type_and_size (gint, glyphv *);
gint find_keepers (gint ncols_current, gint nc, gint *cols, gint *keepers);
gint find_nearest_point (icoords *, splotd *, datad *, ggobid *);
GList* g_list_remove_nth (GList *, gint);
GList* g_list_replace_nth (GList *, gpointer, gint);
void get_extended_brush_corners (icoords *, icoords *, datad *, ggobid *);
GtkItemFactory *get_main_menu (GtkItemFactoryEntry[], gint, GtkAccelGroup *, GtkWidget  *, GtkWidget **, gpointer);
ggobid* ggobi_alloc (void);
gboolean ggobi_file_set_create (gchar *rootname, datad *, ggobid *);
void globals_init (ggobid *);
void hidden_alloc (datad *);
void hidden_init (datad *, ggobid *);
gboolean hidden_read (InputDescription *desc, gboolean, datad *, ggobid *);
void identify_event_handlers_toggle (splotd *, gboolean);
void identify_menus_make (ggobid *);
gboolean impute_fixed (gint, datad *, ggobid *);
void impute_random (datad *, ggobid *);
void impute_window_open (ggobid *);
void init_plot_GC (GdkWindow *, ggobid *);
void init_var_GCs (GtkWidget *, ggobid *);
gfloat jitter_randval (gint);
void jitter_value_set (gfloat, datad *, ggobid *);
void jitter_vars_init (datad *, ggobid *);
void jitter_window_open (ggobid *);
void limits_adjust (gfloat *, gfloat *);
void limits_set (gboolean do_raw, gboolean do_tform, datad *);  
void line_brush_prev_vectors_update (datad *d, ggobid *);
void line_brush_undo (splotd *, datad *, ggobid *);
gboolean line_colors_read (InputDescription *, gboolean, datad *, ggobid *);
void lineedit_event_handlers_toggle (splotd *, gboolean);
void make_symbol_window (ggobid *);
void make_ui (ggobid *);
GlyphType mapGlyphName (const gchar *gtype);
void missing_alloc (gint, gint);
void missing_arrays_add_cols (gint, datad *d, ggobid *);
void missing_block_alloc (gint, gint);
void missing_jitter_value_set (gfloat, datad *, ggobid *);
void missing_lim_set (datad *, ggobid *);
void missing_rejitter (datad *, ggobid *);
void missing_to_world (datad *, ggobid *);
gboolean missing_values_read (InputDescription *desc, gboolean, datad *, ggobid *);
void missing_world_alloc (datad *, ggobid *);
void missing_world_free (datad *d, ggobid *);
void mode_activate (splotd *, gint, gboolean, ggobid *);
gint mode_get (ggobid *);
void mode_set (gint, ggobid *);
void mode_set_cb (GtkWidget  *, gint);
void mode_submenus_activate (splotd *, gint, gboolean, ggobid *);
gboolean mouseinwindow (splotd *);
void mousepos_get_motion (GtkWidget *, GdkEventMotion *, gboolean *, gboolean *, splotd *);
void mousepos_get_pressed (GtkWidget *, GdkEventButton *, gboolean *, gboolean *, splotd *);
void move_pt (gint id, gint x, gint y, splotd *sp, datad *d, ggobid *);
void movepts_event_handlers_toggle (splotd *, gboolean);
void movepts_history_add (gint id, splotd *sp, datad *, ggobid *);
void movepts_history_delete_last (datad *, ggobid *);
gdouble myrint (gdouble x);
FILE* open_ggobi_file_r (gchar *, gint, gchar **, gboolean);
gint option_menu_index (GtkOptionMenu *);
void p1d_reproject (splotd *, glong **, datad *, ggobid *);
gboolean p1d_varsel (splotd *, gint, gint *, gint);
void pan_by_drag (splotd *, ggobid *);
void pan_step (splotd *, gint, ggobid *);
void parcoords_cpanel_init (cpaneld*, ggobid *);
void parcoords_main_menus_make (GtkAccelGroup *, GtkSignalFunc, ggobid *gg, gboolean);
displayd* parcoords_new (gboolean, gint, gint *, datad *, ggobid *);
void parcoords_reset_arrangement (displayd *, gint, ggobid *);
gboolean parcoords_varsel (cpaneld *, splotd *, gint, gint *, ggobid *);
gboolean pca_calc (datad *, ggobid *);
void pca_diagnostics_set (datad *d, ggobid *);
gint pcompare (const void *, const void *);
void pipeline_arrays_add_column (gint, datad *, ggobid *);
void pipeline_arrays_alloc (datad *, ggobid *);
void pipeline_arrays_free (datad *d, ggobid *);
void pipeline_init (datad *, ggobid *);
gint plotted_cols_get (gint *, datad *, ggobid *);
void point_brush_prev_vectors_update (datad *, ggobid *);
void point_brush_undo (splotd *, datad *, ggobid *);
gboolean point_colors_read (InputDescription *desc, gboolean, datad *, ggobid *);
gboolean point_glyphs_read (InputDescription *desc, gboolean, datad *, ggobid *);
gboolean point_in_which_bin (gint, gint, gint *, gint *, datad *, ggobid *);
void populate_option_menu (GtkWidget *, gchar **, gint, GtkSignalFunc, ggobid *);
void position_popup_menu (GtkMenu *menu, gint *px, gint *py, gpointer);
gint projection_get (ggobid *);
void quick_message (gchar *, gboolean);
void quit_ggobi(ggobid *gg, gint action, GtkWidget *w);
gdouble randvalue (void);
void raw_to_tform_copy (void);
gboolean read_ascii_data(InputDescription *desc, ggobid *gg);
void rejitter (datad *, ggobid *);
void rnorm2 (gdouble *, gdouble *);
void rotation_event_handlers_toggle (splotd *, gboolean);
void rotation_menus_make (ggobid *);
void rowlabels_alloc (datad *d, ggobid *) ;
void rowlabels_free (datad *d, ggobid *);
gboolean rowlabels_read (InputDescription *desc, gboolean, datad *, ggobid *);
void rows_in_plot_set (datad *d, ggobid *);
void ruler_ranges_set (displayd *, splotd *, ggobid *);
void scale_event_handlers_toggle (splotd *, gboolean);
void scale_menus_make (ggobid *);
void scaling_visual_cues_draw (splotd *, ggobid *);
void scatmat_cpanel_init (cpaneld *, ggobid *);
void scatmat_main_menus_make (GtkAccelGroup *, GtkSignalFunc, ggobid *gg, gboolean);
displayd* scatmat_new (gboolean, gint, gint *, gint, gint *, datad *, ggobid *);
gboolean scatmat_varsel (cpaneld *, splotd *, gint, gint *, gint, gboolean, ggobid *);
gboolean scatmat_varsel_simple (cpaneld *, splotd *, gint, gint *, ggobid *);
void scatterplot_cpanel_init (cpaneld *, gint, ggobid *);
void scatterplot_main_menus_make (GtkAccelGroup *, GtkSignalFunc, ggobid *gg, gboolean);
displayd* scatterplot_new (gboolean, splotd *sp, datad *d, ggobid *);
void scatterplot_show_rulers (displayd *, gint);
gboolean scree_mapped_p (ggobid *);
void scree_plot_make (datad *, ggobid *);
gint selected_cols_get (gint *, datad *d, ggobid *);
void smooth_window_open (ggobid *);
void sp_event_handlers_toggle (splotd *, gboolean);
void sp_whiskers_make (splotd *, displayd *, ggobid *);
void sphere_condnum_set (gfloat x, ggobid *);
void sphere_enable (gboolean sens, ggobid *);
void sphere_free (datad *);
void sphere_init (datad *);
gint sphere_npcs_get (datad *, ggobid *);
void sphere_npcs_set (gint, datad *, ggobid *);
void sphere_panel_open (ggobid *);
void splot_add_point_label (splotd *, gint, gboolean, ggobid *);
void splot_dimension_set(splotd* sp, gint width, gint height);
void splot_draw_border (splotd *, ggobid *);
void splot_expose (splotd *);
void splot_free (splotd *, displayd *, ggobid *);
void splot_get_dimensions (splotd *, gint *, gint *);
splotd* splot_new (displayd *, gint, gint, ggobid *);
void splot_pixmap0_to_pixmap1 (splotd *, gboolean, ggobid *);
void splot_pixmap1_to_window (splotd *, ggobid *);
void splot_plane_to_screen (displayd *, cpaneld *, splotd *, ggobid *);
void splot_plane_to_world (splotd *, gint, ggobid *);
gboolean splot_plot_case (gint m, datad *, splotd *, displayd *, ggobid *);
void splot_point_colors_used_get (splotd *, gint *ncolors_used, gushort *colors_used, gboolean binned, ggobid *); 
void splot_redraw (splotd *sp, gint, ggobid *);
void splot_reverse_pipeline (splotd *, gint, lcoords *, gboolean, gboolean, ggobid *);
void splot_screen_to_tform (cpaneld *, splotd *, icoords *, fcoords *, ggobid *);
void splot_set_current (splotd *, gboolean, ggobid *);
void splot_set_plot_center (splotd *);
void splot_world_to_plane (cpaneld *, splotd *, ggobid *);
void splot_zoom (splotd *sp, gfloat xsc, gfloat ysc, ggobid *) ;
gint sqdist (gint, gint, gint, gint);
void strip_suffixes (ggobid *);
void submenu_append (GtkWidget *, GtkWidget *);
void submenu_destroy (GtkWidget *);
void submenu_insert (GtkWidget *, GtkWidget *, gint);
GtkWidget* submenu_make (gchar *, gint, GtkAccelGroup *);
void subset_apply (gboolean, datad *, ggobid *);
gboolean subset_block (gint, gint, datad *, ggobid *);
gboolean subset_everyn (gint, gint, datad *, ggobid *);
void subset_include_all (datad *, ggobid *);
gboolean subset_random (gint, datad *, ggobid *);
gboolean subset_rowlab (gchar *, datad *, ggobid *);
gboolean subset_sticky (datad *, ggobid *);
void subset_window_open (ggobid *, guint, GtkWidget *);
void textur (gfloat *, gfloat *, gint, gint, gfloat, gint, ggobid *);
void tform1_to_tform2_copy (void);
void tform_label_update (gint, datad *, ggobid *);
void tform_to_world (datad *, ggobid *);
void tform_to_world_by_var (gint j, datad *, ggobid *);
void tour1d_event_handlers_toggle (splotd *, gboolean);
void tour1d_func (gboolean, displayd *, ggobid *);
void tour1d_menus_make (ggobid *);
void tour1d_projdata (splotd *, glong **, datad *, ggobid *);
void tour1d_varsel (ggobid *, gint jvar, gint btn);
void tour1d_vert (cpaneld *, gboolean);
void tour1dpp_window_open (ggobid *);
void tour1dpp_window_open (ggobid *);
void tour2d_event_handlers_toggle (splotd *, gboolean);
void tour2d_func (gboolean, displayd *, ggobid *);
void tour2d_menus_make (ggobid *);
void tour2d_projdata (splotd *, glong **, datad *, ggobid *);
void tour2dpp_window_open (ggobid *);
void tour_do_step (displayd *, ggobid *);
void tour2d_realloc_up (gint nc, datad *d, ggobid *gg);
void tour2d_realloc_down (gint nc, gint *cols, datad *d, ggobid *gg);
void tour1d_realloc_up (gint nc, datad *d, ggobid *gg);
void tour1d_realloc_down (gint nc, gint *cols, datad *d, ggobid *gg);
void tourcorr_realloc_up (gint nc, datad *d, ggobid *gg);
void tourcorr_realloc_down (gint nc, gint *cols, datad *d, ggobid *gg);
void tourcorr_event_handlers_toggle (splotd *, gboolean);
void tourcorr_func (gboolean, displayd *, ggobid *);
void tourcorr_menus_make (ggobid *);
void tourcorr_projdata (splotd *, glong **, datad *, ggobid *);
void tourcorr_varsel (ggobid *, gint jvar, gint btn);
void transform (gint, gint, gfloat, datad *, ggobid *);
void transform0_opt_menu_set_value (gint j, datad *d, ggobid *gg);
void transform0_values_set (gint, gint, datad *, ggobid *);
gboolean transform1_apply (gint, datad *, ggobid *);
void transform1_opt_menu_set_value (gint j, datad *d, ggobid *gg);
void transform1_values_set (gint, gfloat, gint, datad *, ggobid *);
gboolean transform2_apply (gint, datad *, ggobid *);
void transform2_opt_menu_set_value (gint j, datad *d, ggobid *gg);
void transform2_values_set (gint, gint, datad *, ggobid *);
void transform_values_init (gint j, datad *, ggobid *);
void transform_variable (gint, gint, gfloat, gint, datad *, ggobid *);
void transform_window_open (ggobid *);
void varcircle_label_set (gint, datad *);
void varcircles_add (gint ncols, datad *, ggobid *);
void varcircles_delete_nth (gint j, datad *);
void varcircles_layout_reset (gint ncols, datad *, ggobid *);
void varcircles_populate (datad *, ggobid *);
void varcircles_refresh (datad *, ggobid *);
void vardialog_open (ggobid *, gchar *title);
void variable_clone (gint, const gchar *, gboolean, datad *, ggobid *);
void varlabel_set (gint, datad *);
void varpanel_checkbox_add (gint j, datad *d, ggobid *gg);
void varpanel_make (GtkWidget *, ggobid *);
void varpanel_populate (datad *, ggobid *);
void varpanel_refresh (ggobid *);
void varpanel_tooltips_set (ggobid *);
void varsel (cpaneld *, splotd *, gint jvar, gint btn, gint alt_mod, gint ctrl_mod, gint shift_mod, datad *, ggobid *);
void vartable_alloc (datad *);
void vartable_collab_tform_set_by_var (gint j, datad *d);
void vartable_init (datad *d);
void vartable_limits_set (datad *);
void vartable_limits_set_by_var (gint j, datad *d);
void vartable_open (ggobid *);
void vartable_realloc (gint, datad *);
void vartable_row_append (gint, datad *, ggobid *);
void vartable_select_var (gint, gboolean, datad *, ggobid *);
void vartable_stats_set (datad *);
void vartable_stats_set_by_var (gint j, datad *);
void vectorb_alloc (vector_b *, gint);
void vectorb_copy (vector_b *, vector_b *);
void vectorb_free (vector_b *);
void vectorb_init (vector_b *);
void vectorb_realloc (vector_b *, gint);
void vectorb_realloc_zero (vector_b *, gint);
void vectorf_alloc (vector_f *, gint);
void vectorf_alloc_zero (vector_f *, gint);
void vectorf_delete_els (vector_f *vecp, gint nels, gint *els);
void vectorf_free (vector_f *);
void vectorf_init (vector_f *);
void vectorf_realloc (vector_f *, gint);
void vectorf_zero (vector_f *vecp);
void vectori_alloc (vector_i *, gint);
void vectori_alloc_zero (vector_i *, gint);
void vectori_copy (vector_i *, vector_i *);
void vectori_delete_els (vector_i *vecp, gint nels, gint *els);
void vectori_free (vector_i *);
void vectori_init (vector_i *);
void vectori_realloc (vector_i *, gint);
void vectors_copy (vector_s *, vector_s *);
void vectors_free (vector_s *);
void vectors_init (vector_s *);
void vectors_realloc (vector_s *, gint);
void vectors_realloc_zero (vector_s *, gint);
/* void tour_reproject (splotd *, glong **, datad *, ggobid *);*/
void widget_initialize (GtkWidget *w, gboolean initd);
gboolean widget_initialized (GtkWidget *w);
void world_to_raw (gint, splotd *, datad *, ggobid *);
void writeall_window_open (ggobid *);
void xy_reproject (splotd *, glong **, datad *, ggobid *);
gboolean xyplot_varsel (splotd *, gint, gint *, gint);
void zoom_by_drag (splotd *, ggobid *);
void zoom_step (splotd *, gint, gint, rectd *, ggobid*);


ggobid * ggobi_get (gint);
gint ggobi_getIndex(ggobid *gg);

displayd * display_alloc_init (enum displaytyped, gboolean, datad *, ggobid *);

/*tsplot functions*/
extern void cpanel_tsplot_init (cpaneld *, ggobid *);
extern void cpanel_tsplot_make (ggobid *);
extern void tsplot_cpanel_init (cpaneld*, ggobid *);
extern void tsplot_main_menus_make (GtkAccelGroup *, GtkSignalFunc, ggobid *gg, gboolean);
extern displayd* tsplot_new (gboolean, gint, gint *, datad *, ggobid *);
extern void tsplot_reset_arrangement (displayd *, gint, ggobid *);
extern gboolean tsplot_varsel (cpaneld *, splotd *, gint, gint, gint *, 
                               ggobid *);
/* The new way of handling window closures, so that we don't just exit. */
void ggobi_close (GtkObject *w, ggobid *);
int ggobi_remove_by_index (ggobid *gg, gint which);
int ggobi_remove (ggobid *);


void showInputDescription(InputDescription *desc, ggobid *gg);
void initSessionOptions();
gint parse_command_line (gint *argc, gchar **av, ggobid *gg);
#ifdef __cplusplus
}
#endif

#include "GGobiAPI.h"

#define CHECK_GG(a) ValidateGGobiRef(a, true)

#endif

