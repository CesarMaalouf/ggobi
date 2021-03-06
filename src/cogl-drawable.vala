/* Implement Drawable with Cogl and OpenGL */

/* Cogl is an abstraction over OpenGL and OpenGL ES. The abstraction
   is not useful to GGobi, as it does not target embedded
   systems. However, Cogl does offer some utilities on top of OpenGL,
   although most of them are aimed at eye candy apps, like media
   centers. Currently, Cogl is used for clipping and text rendering
   (via ClutterPango).
*/

/* note that setting line join, cap and miter not supported */

/* Ideas:
*/

/* Issues:
   - the stencil buffer does not work on the FBO. Should we try
   packing the stencil into the depth buffer?
*/
using GL;
using Pango;

public class GGobi.Surface.CoglDrawable : Drawable, Object {
  
  public uint width { get; set construct; }
  public uint height { get; set construct; }

  private float max_point_size;
  
  construct {
    // FIXME: this will be run every draw, so this should only change
    // state for the majority of the operations. Special case state
    // tweaks belong elsewhere. Much of this should be moved.
    // FIXME: do we really want to change state at
    // construction/deconstruction? maybe we need a start_drawing(),
    // end_drawing() type of thing, which would be called by the
    // surface for the geom's convenience
    PushClientAttrib(Consts.CLIENT_VERTEX_ARRAY_BIT);
    EnableClientState(Consts.VERTEX_ARRAY);
    /* OGL 1.4 */
    /* The ordinary BlendFunc() applies the same blending to the
       alpha channel as the RGB channels. The below emulates Cairo. */
    PushAttrib(Consts.COLOR_BUFFER_BIT | Consts.ENABLE_BIT);
    BlendFuncSeparate(Consts.SRC_ALPHA, Consts.ONE_MINUS_SRC_ALPHA,
                      Consts.ONE, Consts.ONE_MINUS_SRC_ALPHA);
    GLfloat[] p = new GLfloat[2];
    GetFloatv(Consts.POINT_SIZE_RANGE, p);
    max_point_size = p[1];
    Enable(Consts.POINT_SMOOTH);
  }
  
  public override void finalize() {
    PopClientAttrib();
    PopAttrib();
    base.finalize();
  }
  
  public CoglDrawable(uint width, uint height) {
    this.width = width;
    this.height = height;
  }

  private Color stroke;
  private Color fill;
  
  /* configure colors */

  private void set_color(Color color) {
    Color4f(color.red, color.green, color.blue, color.alpha);
  }
  
  public void set_stroke(Color? color) {
    /* cannot set 'stroke' to NULL, as it is non-nullable. We could
       make it nullable, but then it would be dynamically allocated in
       C. To avoid that overhead, we set alpha to 0 to indicate null. */
    if (color == null)
      stroke.alpha = 0;
    else stroke = color;
  }
  public void set_fill(Color? color) {
    if (color == null)
      fill.alpha = 0;
    else fill = color;
  }
  
  /* configure line parameters */
  public void set_line_width(uint width) {
    LineWidth(width);
  }
  
  public void set_dashes(int factor, ushort pattern) {
    if (factor == 0)
      Disable(Consts.LINE_STIPPLE);
    Enable(Consts.LINE_STIPPLE);
    LineStipple(factor, pattern);
  }

  /* set clip */
  public void set_clip(int x, int y, uint width, uint height) {
    Cogl.Clip.set(x, y, (int)width, (int)height);
  }
  public void unset_clip() {
    Cogl.Clip.unset();
  }
  
  /* draw stuff */
  public void draw_rectangle(int x, int y, uint width, uint height) {
    //g_debug("drawing rect: %d %d %d %d", x, y, width, height);
    if (fill.alpha > 0) {
      set_color(fill);
      Recti((GLint)(x - 1), (GLint)(y - 1), (GLint)(x + width - 2),
            (GLint)(y + height - 2));
    }
    if (stroke.alpha > 0) {
      set_color(stroke);
      Begin(Consts.LINE_LOOP);
      Vertex2f(x, y);
      Vertex2f(x, y + (int)height);
      Vertex2f(x + (int)width, y + (int)height);
      Vertex2f(x + (int)width, y);
      End();
    }
  }

  public void draw_circle(int cx, int cy, uint r) {
    
    /* Optimization:
       If radius is within point size range and circle is filled:
         Draw a size 2(r-1) anti-aliased point.
       This takes about half the time as drawing the circle from
       scratch, and it looks prettier. There is a lot of overhead that
       could be avoided if many circles were drawn with the same
       radius, but that is the domain of the glyph rendering.
    */
    
    if (max_point_size >= 2*(r-1)) {
      if (fill.alpha > 0) {
        //Enable(Consts.POINT_SMOOTH);
        set_color(fill);
        PointSize(2*(r-1));
        Begin(Consts.POINTS);
        Vertex2f(cx, cy);
        End();
        //Disable(Consts.POINT_SMOOTH);
      }
    } else {
      if (fill.alpha > 0) {
        set_color(fill);
        midpoint_circle(cx, cy, r - 1, true);
      }
      if (stroke.alpha > 0) {
      // FIXME: does not obey line width
        set_color(stroke);
        midpoint_circle(cx, cy, r, false);
      }
    }
  }

  private void midpoint_circle(int cx, int cy, uint r, bool fill) {
    /* midpoint circle algorithm, implementation adapted from:
       http://en.wikipedia.org/wiki/Midpoint_circle_algorithm */
    /* WISH: some way to guess final value of 'x' to allocate an array */
    
    int f = 1 - (int)r;
    int ddF_x = 1;
    int ddF_y = -2 * (int)r;
    int x = 0;
    int y = (int)r, fy;
    int tx = 0;
    int[] v = null;
    int j = 0;

    if (fill) { // not clear why this correction is needed
      cy++;
    }
      
    while(x < y) {  /* need to preallocate array */
      if(f >= 0) {
        y--;
        ddF_y += 2;
        f += ddF_y;
      }
      x++;
      ddF_x += 2;
      f += ddF_x;
    }

    if (!fill) {
      tx = 2*x;
      v = new int[8*(tx+1)];
      v[0] = cx; v[1] = cy + (int)r;
      v[2] = cx; v[3] = cy - (int)r;
      v[4] = cx + (int)r; v[5] = cy;
      v[6] = cx - (int)r; v[7] = cy;
      j = 8;
    }

    if (fill) {
      v = new int[8*x + 8*(r - y + 1) + 4];
      v[v.length - 4] = cx - (int)r; v[v.length - 3] = cy;
      v[v.length - 2] = cx + (int)r + 1; v[v.length - 1] = cy;
    }

    y = (int)r;
    f = 1 - (int)r;
    ddF_x = 1;
    ddF_y = -2 * (int)r;
    x = 0;
    
    while(x < y) {
      if(f >= 0) {
        if (fill) {
          v[j++] = cx - x; v[j++] = cy + y;
          v[j++] = cx + x + 1; v[j++] = cy + y;
          v[j++] = cx - x; v[j++] = cy - y;
          v[j++] = cx + x + 1; v[j++] = cy - y;
        }
        y--;
        fy--;
        ddF_y += 2;
        f += ddF_y;
      }
      x++; // not clear why 'fill' correction needed here
      v[j++] = cx + y + (int)fill; v[j++] = cy + x;
      v[j++] = cx - y; v[j++] = cy + x;
      v[j++] = cx + y + (int)fill; v[j++] = cy - x;
      v[j++] = cx - y; v[j++] = cy - x;
      if (!fill) {
        v[j++] = cx - x; v[j++] = cy + y;
        v[j++] = cx + x; v[j++] = cy + y;
        v[j++] = cx - x; v[j++] = cy - y;
        v[j++] = cx + x; v[j++] = cy - y;
      }
      ddF_x += 2;
      f += ddF_x;
    }

    if (r > 0 && fill) {
      v[j++] = cx - x; v[j++] = cy + y;
      v[j++] = cx + x; v[j++] = cy + y;
      v[j++] = cx - x; v[j++] = cy - y;
      v[j++] = cx + x; v[j++] = cy - y;
    }
    
    VertexPointer(2, Consts.INT, 0, v);
    
    if (fill)
      DrawArrays(Consts.LINES, 0, (GLsizei)(v.length/2.0));
    else DrawArrays(Consts.POINTS, 0, (GLsizei)(v.length/2.0));
  }
  
  public void draw_polyline(int[] x, int[] y) {
    vertices(Consts.LINE_STRIP, x, y);
  }
  public void draw_polygon(int[] x, int[] y) {
    vertices(Consts.POLYGON, x, y);   
  }

  /* fast paths */
  public void draw_points(int[] x, int[] y) {
    vertices(Consts.POINTS, x, y);
  }
  public void draw_segments(int[] x, int[] y) {
    vertices(Consts.LINES, x, y);
  }

  /* hopefully PangoClutter stuff is temporary -
     Cairo needs a working OpenGL backend */
  private static Pango.FontMap font_map = new Pango.Clutter.FontMap();
  
  private Context pango_context =
    ((Pango.Clutter.FontMap)font_map).create_context();
  
  /* configure font */
  /* these will be set on PangoContext */
  public void set_font_family(string family) {
    weak FontDescription desc = pango_context.get_font_description();
    desc.set_family(family);
  }
  public void set_font_size(uint size) {
    weak FontDescription desc = pango_context.get_font_description();
    desc.set_size(Pango.SCALE*(int)size);
  }
  public void set_font_style(FontStyle style) {
    weak FontDescription desc = pango_context.get_font_description();
    Style pango_style;
    switch(style) {
    case FontStyle.OBLIQUE:
      pango_style = Style.OBLIQUE;
      break;
    case FontStyle.ITALIC:
      pango_style = Style.ITALIC;
      break;
    default:
      pango_style = Style.NORMAL;
      break;
    }
    desc.set_style((Style)style);
  }
  public void set_font_weight(FontWeight weight) {
    weak FontDescription desc = pango_context.get_font_description();
    Weight pango_weight;
    switch(weight) {
    case FontWeight.BOLD:
      pango_weight = Weight.BOLD;
      break;
    default:
      pango_weight = Weight.NORMAL;
      break;
    }
    desc.set_weight(pango_weight);
  }

  /* querying font */
  public void font_extents(out int ascent, out int descent) {
    weak FontDescription desc = pango_context.get_font_description();
    FontMetrics metrics = pango_context.get_metrics(desc, null);
    ascent = metrics.get_ascent();
    descent = metrics.get_descent();
  }
  public void text_extents(string str, out uint width, out uint height) {
    Layout layout = layout_text(str);
    Rectangle extents;
    layout.get_line(0).get_pixel_extents(null, out extents);
    width = extents.width;
    height = extents.height;
  }

  /* draw text */

  public void set_text_rotation(double rot) {
    Pango.Matrix matrix = Pango.Matrix() { xx=1, xy=0, yx=0, yy=1, x0=0, y0=0 };
    matrix.rotate(rot);
    pango_context.set_matrix(matrix);
  }
  
  public void draw_text(string str, int x, int y) {
    /* using a PangoLayout */
    if (fill.alpha > 0) {
      Layout layout = layout_text(str);
      Pango.Clutter.render_layout(layout, x, y, to_clutter_color(fill), 0);
    }
  }

  private Clutter.Color to_clutter_color(Color color) {
    Clutter.Color clutter = Clutter.Color();
    clutter.red = (uchar) (color.red * 255);
    clutter.blue = (uchar) (color.blue * 255);
    clutter.green = (uchar) (color.green * 255);
    clutter.alpha = (uchar) (color.alpha * 255);
    return clutter;
  }

  // Plan: write a simple glyph -> texture translator and cache. Use
  // point sprites to blit the textures on screen.
  public void draw_glyphs(string glyph, int[] x, int[] y) {
    /* no glyphs are drawn if there is no fill */
    if (fill.alpha == 0)
      return;
    Layout layout = layout_text(glyph);
    Clutter.Color text_color = to_clutter_color(fill);
    /* get glyph out of layout, lookup texture in cache, draw it all over */
    
    for (int i = 0; i < x.length; i++)
      Pango.Clutter.render_layout(layout, x[i], y[i], text_color, 0);
    /* Point sprite stuff
       Enable(Consts.TEXTURE_2D);
       Enable(Consts.POINT_SPRITE);
       TexEnvi(Consts.POINT_SPRITE, Consts.COORD_REPLACE, 1);
       PointSize(10);
       PointParameteri(Consts.POINT_SPRITE_COORD_ORIGIN, Consts.LOWER_LEFT);
       //Enable(Consts.POINT_SMOOTH);
    */

  }

  private Layout layout_text(string str) {
    Layout layout = new Layout(pango_context);
    layout.set_text(str, -1);
    return layout;
  }
  
  private void vertices(GLenum mode, int[] x, int[] y)
  {
    /*
    set_color(stroke);
    Begin(mode);
    for (int i = 0; i < x.length; i++) {
      Vertex2f(x[i], y[i]);
    }
    End();
    */
    
    int[] vertices = new int[x.length*2];
    for (int i = 0; i < x.length; i++) {
      vertices[i*2] = x[i];
      vertices[i*2+1] = y[i];
    }
    VertexPointer(2, Consts.INT, 0, vertices);
    if (mode == Consts.POLYGON) {
      if (fill.alpha > 0) {
        set_color(fill);
        DrawArrays(Consts.POLYGON, 0, (GLsizei)(x.length));
      }
      if (stroke.alpha > 0)
        mode = Consts.LINE_LOOP;
    }
    if (mode != Consts.POLYGON) {
      set_color(stroke);
      DrawArrays(mode, 0, (GLsizei)(x.length));
    }
    
  }
}