#include <gtk/gtk.h>
#include "ggobi.h"
#include "externs.h"
#include "GGobiAPI.h"

#include <stdio.h>
#include <math.h>

#include "plugin.h"
#include "ggvis.h"

/*-- utility --*/

static 
GList * list_concat_uniq (GList *a, GList *b)
{
  GList *l, *ab = NULL;
  noded *n;

  for (l = a; l; l = l->next) {
    n = (noded *) l->data;
    if (g_list_index (ab, n) == -1)
      ab = g_list_append (ab, n);
  }
  for (l = b; l; l = l->next) {
    n = (noded *) l->data;
    if (g_list_index (ab, n) == -1)
      ab = g_list_append (ab, n);
  }

  return ab;
}

static 
GList * list_subset_uniq (GList *full)
{
  GList *l, *uniq = NULL;
/*
 * If a list element is not already a member of the unique list,
 * add it.
*/

  for (l = full; l; l = l->next) {
    if (g_list_position (uniq, l) == -1)
      uniq = g_list_append (uniq, l->data);
  }

  return uniq;
}

static
void list_clear (GList *ab)
{
  GList *l;
  
  for (l = ab; l; l = l->next)
    ab = g_list_remove_link (ab, l);
  ab = NULL;
}

/* unused */
gboolean
hasPathToCenter (noded *n, noded *referringnode, datad *d, datad *e,
  PluginInstance *inst)
{
  gboolean hasPath = false;
  gint k;
  noded *n1;
  ggvisd *ggv = GGVisFromInst (inst);
  noded *centerNode = ggv->radial->centerNode;
  GList *l, *connectedEdges = list_subset_uniq (n->connectedEdges);

  for (l = connectedEdges; l; l = l->next) {
    k = GPOINTER_TO_INT (l->data);

    /*-- if edge[k] is included and visible ... --*/
    if (e->sampled.els[k] && !d->hidden.els[k]) {

      n1 = &ggv->radial->nodes[ d->rowid.idv.els[e->edge.endpoints[k].a] ];
      if (n1->i == n->i)
        n1 = &ggv->radial->nodes[ d->rowid.idv.els[e->edge.endpoints[k].b] ];

      if (referringnode != NULL && n1->i == referringnode->i)
        continue;  /*-- skip over this node; we've already tested it --*/
      
      /*-- if n1 is included and visible ... --*/
      if (d->sampled.els[n1->i] && !d->hidden.els[n1->i]) {
        /*-- if n1 is no farther from the center than n ... --*/
        if (n1->nStepsToCenter <= n->nStepsToCenter) {
          /*-- if n1 is the center node or has a path to it ... --*/
          if (n1->i == centerNode->i || hasPathToCenter (n1, n, d, e, inst)) {
            hasPath = true;
            break;
          }
        }
      }
    }
  }

  list_clear (connectedEdges);
  return hasPath;
}


/*-----------------------------------------------------------------*/
/*                   callbacks                                     */
/*-----------------------------------------------------------------*/

void radial_cb (GtkButton *button, PluginInstance *inst)
{
  ggobid *gg = inst->gg;
  ggvisd *ggv = GGVisFromInst (inst);
  datad *d = gg->current_display->d;
  datad *e = gg->current_display->e;
  gboolean init;
/*-- to add variables --*/
  gint i, k, nP, nC, nS;
  gdouble *x, *y, *depth, *inDegree, *outDegree;
  gdouble *nParents, *nChildren, *nSiblings;
  gchar *name;
  GList *l, *connectedNodes;
  noded *n, *n1;

  if (d == NULL || e == NULL) {
    quick_message ("Please specify an edge set", false);
    return;
  }
  if (!d->sampled.els[0] || d->hidden.els[0]) {
    g_printerr ("Trouble: you've eliminated the center node.\n");
    return;
  }

/*-- This may not belong here, but where exactly?  As soon as the
     panel is opened  --*/
  gtk_signal_connect (GTK_OBJECT(gg->main_window),
    "sticky_point_added", highlight_sticky_edges, inst);
  gtk_signal_connect (GTK_OBJECT(gg->main_window),
    "sticky_point_removed", highlight_sticky_edges, inst);

  init = (ggv->radial == NULL || d->nrows != ggv->radial->d->nrows);
  initLayout (gg, ggv, d, e);

  /*-- initial default:  let the first node be the center node --*/
  ggv->radial->centerNode = &ggv->radial->nodes[0];
  ggv->radial->centerNode->i = 0;

  setParentNodes (ggv, d);
  setNChildren (ggv, d);
  setSubtreeSize (ggv->radial->centerNode, ggv, d);
  setSubtreeSpans (ggv, d);
  setNodePositions (ggv, d);

/*-- add variables generated by the layout algorithm --*/
  x = g_malloc0 (d->nrows * sizeof (gdouble));
  y = g_malloc0 (d->nrows * sizeof (gdouble));
  depth = g_malloc0 (d->nrows * sizeof (gdouble));
  inDegree = g_malloc0 (d->nrows * sizeof (gdouble));
  outDegree = g_malloc0 (d->nrows * sizeof (gdouble));
  nParents = g_malloc0 (d->nrows * sizeof (gdouble));
  nChildren = g_malloc0 (d->nrows * sizeof (gdouble));
  nSiblings = g_malloc0 (d->nrows * sizeof (gdouble));

  for (i=0; i<d->nrows; i++) {  /*-- nrows = nnodes --*/
    if (!d->sampled.els[i] || d->hidden.els[i]) {
      x[i] = y[i] = depth[i] = 0.0;
      inDegree[i] = outDegree[i] = 0.0;
      nChildren[i] = nParents[i] = nSiblings[i] = 0.0;
    } else {

      n = &ggv->radial->nodes[i];

      /*-- there's no reason to draw these points; they're orphans --*/
      if (n->nStepsToCenter == -1)
        d->hidden.els[i] = d->hidden_now.els[i] = true;

      x[i] = n->pos.x;
      y[i] = n->pos.y;
      depth[i] = (gdouble) n->nStepsToCenter;
      inDegree[i] = (gdouble) n->inDegree;
      outDegree[i] = (gdouble) n->outDegree;

      connectedNodes = list_subset_uniq (n->connectedNodes);

      nP = nC = nS = 0;
      for (l = connectedNodes; l; l = l->next) {
        n1 = (noded *) l->data;

        if (!d->sampled.els[ n1->i ] || d->hidden.els[ n1->i])
          continue;
        if (n1->nStepsToCenter == -1)
          continue;

        if (n1->nStepsToCenter == n->nStepsToCenter)
          nS++;
        else if (n1->nStepsToCenter < n->nStepsToCenter)
          nP++;
        else if (n1->nStepsToCenter > n->nStepsToCenter)
          nC++;
      }
      list_clear (connectedNodes);

      nChildren[i] = (gdouble) nC;
      nParents[i] = (gdouble) nP;
      nSiblings[i] = (gdouble) nS;
    }
  }

  if (init) {
    name = g_strdup_printf ("x");
    newvar_add_with_values (x, d->nrows, name, d, gg);
    g_free (name);
    g_free (x);

    name = g_strdup_printf ("y");
    newvar_add_with_values (y, d->nrows, name, d, gg);
    g_free (name);
    g_free (y);

    name = g_strdup_printf ("depth");
    newvar_add_with_values (depth, d->nrows, name, d, gg);
    g_free (name);
    g_free (depth);

    name = g_strdup_printf ("in degree");
    newvar_add_with_values (inDegree, d->nrows, name, d, gg);
    g_free (name);
    g_free (inDegree);

    name = g_strdup_printf ("out degree");
    newvar_add_with_values (outDegree, d->nrows, name, d, gg);
    g_free (name);
    g_free (outDegree);

    name = g_strdup_printf ("nparents");
    newvar_add_with_values (nParents, d->nrows, name, d, gg);
    g_free (name);
    g_free (nParents);

    name = g_strdup_printf ("nchildren");
    newvar_add_with_values (nChildren, d->nrows, name, d, gg);
    g_free (name);
    g_free (nChildren);

    name = g_strdup_printf ("nsiblings");
    newvar_add_with_values (nSiblings, d->nrows, name, d, gg);
    g_free (name);
    g_free (nSiblings);
  } else {  /*-- overwrite the values of all those variables --*/
    gint j;

    j = GGOBI(getVariableIndex)("x", d, gg);
    for (i=0; i<d->nrows; i++)
      d->raw.vals[i][j] = d->tform.vals[i][j] = x[i];
    limits_set_by_var (j, true, true, d, gg);
    g_free (x);

    j = GGOBI(getVariableIndex)("y", d, gg);
    for (i=0; i<d->nrows; i++)
      d->raw.vals[i][j] = d->tform.vals[i][j] = y[i];
    limits_set_by_var (j, true, true, d, gg);
    g_free (y);

    j = GGOBI(getVariableIndex)("in degree", d, gg);
    for (i=0; i<d->nrows; i++)
      d->raw.vals[i][j] = d->tform.vals[i][j] = inDegree[i];
    limits_set_by_var (j, true, true, d, gg);
    g_free (inDegree);

    j = GGOBI(getVariableIndex)("out degree", d, gg);
    for (i=0; i<d->nrows; i++)
      d->raw.vals[i][j] = d->tform.vals[i][j] = outDegree[i];
    limits_set_by_var (j, true, true, d, gg);
    g_free (outDegree);

    j = GGOBI(getVariableIndex)("nparents", d, gg);
    for (i=0; i<d->nrows; i++)
      d->raw.vals[i][j] = d->tform.vals[i][j] = nParents[i];
    limits_set_by_var (j, true, true, d, gg);
    g_free (nParents);

    j = GGOBI(getVariableIndex)("nchildren", d, gg);
    for (i=0; i<d->nrows; i++)
      d->raw.vals[i][j] = d->tform.vals[i][j] = nChildren[i];
    limits_set_by_var (j, true, true, d, gg);
    g_free (nChildren);

    j = GGOBI(getVariableIndex)("nsiblings", d, gg);
    for (i=0; i<d->nrows; i++)
      d->raw.vals[i][j] = d->tform.vals[i][j] = nSiblings[i];
    limits_set_by_var (j, true, true, d, gg);
    g_free (nSiblings);

    tform_to_world (d, gg);
    displays_tailpipe (FULL, gg);
  }
}

void highlight_sticky_edges (GtkWidget *w, gint index, gint state, datad *d,
  PluginInstance *inst)
{
  ggobid *gg = inst->gg;
  ggvisd *ggv = GGVisFromInst (inst);
  datad *e = gg->current_display->e;
  noded *n, *n1;
  GList *l, *connectedNodes, *connectedEdges;
  gint k;

  /*-- Do I have to loop over displays, looking for the one(s) with
       edges?  --*/
  if (e == NULL) return;

  n = &ggv->radial->nodes[index];

  connectedNodes = list_subset_uniq (n->connectedNodes);
  connectedNodes = g_list_append (connectedNodes, n);
  for (l = connectedNodes; l; l = l->next) {
    n1 = (noded *) l->data;
    d->color.els[n1->i] = d->color_now.els[n1->i] = 
      (state == STICKY) ? gg->color_id : gg->color_0;
    d->glyph.els[n1->i].size = d->glyph_now.els[n1->i].size = 
      (state == STICKY) ? gg->glyph_id.size : gg->glyph_0.size;
    d->glyph.els[n1->i].type = d->glyph_now.els[n1->i].type = 
      (state == STICKY) ? gg->glyph_id.type : gg->glyph_0.type;
  }
  list_clear (connectedNodes);

  connectedEdges = list_subset_uniq (n->connectedEdges);
  for (l = connectedEdges; l; l = l->next) {
    k = GPOINTER_TO_INT (l->data);
    if (k < 0 || k >= e->nrows)
      break;

    if (state == STICKY) {
      e->color.els[k] = e->color_now.els[k] = gg->color_id;
      e->glyph.els[k].size = e->glyph_now.els[k].size = gg->glyph_id.size;
      e->glyph.els[k].type = e->glyph_now.els[k].type = gg->glyph_id.type;
    } else {
      gint a = d->rowid.idv.els[e->edge.endpoints[k].a];
      gint b = d->rowid.idv.els[e->edge.endpoints[k].b];

      if ((a == index &&
           g_slist_index (d->sticky_ids, GINT_TO_POINTER(b)) != -1) ||
          (b == index &&
           g_slist_index (d->sticky_ids, GINT_TO_POINTER(a)) != -1))
      {
        /*
         * if one of the nodes connected to node k has a sticky label,
         * don't downweight the edge
        */
         ;
      } else {
        e->color.els[k] = e->color_now.els[k] = gg->color_0;
        e->glyph.els[k].size = e->glyph_now.els[k].size = gg->glyph_0.size;
        e->glyph.els[k].type = e->glyph_now.els[k].type = gg->glyph_0.type;
      }
    }
  }
  list_clear (connectedEdges);

  displays_plot (NULL, FULL, gg);
}


/*-----------------------------------------------------------------*/

/*
 * Initialize a couple of values for each node.
*/
void
initLayout (ggobid *gg, ggvisd *ggv, datad *d, datad *e) {
  gint i, k, nn;
  noded *na, *nb;
  gint nnodes = d->nrows;
  gint nedges = e->edge.n;
  endpointsd *endpoints = e->edge.endpoints;
  gint a, b;
  gint nnodessq = nnodes * nnodes;

  if (ggv->radial != NULL) {
    nn = ggv->radial->d->nrows;
    for (i=0; i < nn; i++) {
      g_list_free (ggv->radial->nodes[i].connectedEdges);
      g_list_free (ggv->radial->nodes[i].connectedNodes);
    }
  } else {
    ggv->radial = (radiald *) g_malloc (sizeof (radiald));
    ggv->radial->nodes = (noded *) g_malloc (nnodes * sizeof (noded));
  }
  ggv->radial->d = d;

  for (i = 0; i <nnodes; i++) {
    ggv->radial->nodes[i].connectedEdges = NULL;
    ggv->radial->nodes[i].connectedNodes = NULL;
    ggv->radial->nodes[i].inDegree = 0;
    ggv->radial->nodes[i].outDegree = 0;
    ggv->radial->nodes[i].subtreeSize = 0;
    ggv->radial->nodes[i].nChildren = 0;
    ggv->radial->nodes[i].nStepsToCenter = nnodessq;
    ggv->radial->nodes[i].i = i;
    ggv->radial->nodes[i].parentNode = NULL;

    if (nedges <= 1) {
      ggv->radial->nodes[i].nStepsToLeaf = 0;
    } else {
      ggv->radial->nodes[i].nStepsToLeaf = nnodessq;
    }
  }

  /*-- initialize the linked lists of edges and nodes --*/

  /*-- loop over the edges --*/
  for (i = 0; i <e->edge.n; i++) {
    if (e->sampled.els[i] && !e->hidden.els[i]) {
      a = d->rowid.idv.els[endpoints[i].a];
      b = d->rowid.idv.els[endpoints[i].b];
      if (d->sampled.els[a] && !d->hidden.els[a] &&
          d->sampled.els[b] && !d->hidden.els[b])
      {
        na = &ggv->radial->nodes[a];
        nb = &ggv->radial->nodes[b];

        nb->connectedNodes = g_list_append (nb->connectedNodes, na);
        nb->connectedEdges = g_list_append (nb->connectedEdges,
          GINT_TO_POINTER (i));
        nb->inDegree++;

        na->connectedNodes = g_list_append (na->connectedNodes, nb);
        na->connectedEdges = g_list_append (na->connectedEdges,
          GINT_TO_POINTER (i));
        na->outDegree++;
      }
    }
  }
}

void
setNStepsToCenter (noded *n, noded *prevNeighbor, datad *d) {
  noded *n1;
  gint nsteps = n->nStepsToCenter + 1;
  GList *l;
  GList *connectedNodes = list_subset_uniq (n->connectedNodes);

  /*-- source nodes: ie, the edge originates here --*/
  for (l = connectedNodes; l; l = l->next) {
    n1 = (noded *) l->data;
    if (prevNeighbor != NULL && n1->i == prevNeighbor->i)
      continue;

    if (nsteps < n1->nStepsToCenter) {
      n1->nStepsToCenter = nsteps;
      n1->parentNode = n;
      setNStepsToCenter (n1, n, d);
    }
  }

  list_clear (connectedNodes);
}


/*
 * Work out from the center and determine the value of
 * nStepsToCenter and parent node for each node.
*/
void
setParentNodes (ggvisd *ggv, datad *d) {
  gint m, i;
  noded *n;
  gint nnodessq = d->nrows * d->nrows;

  noded *centerNode = ggv->radial->centerNode;

  centerNode->nStepsToCenter = 0;
  centerNode->parentNode = NULL;
  setNStepsToCenter (centerNode, NULL, d);

  // find the maximum number of steps from the center
  ggv->radial->nStepsToCenter = 0;
  for (m=0; m<d->nrows_in_plot; m++) {
    i = d->rows_in_plot[m];
    n = &ggv->radial->nodes[i];

    /*-- these guys have no path to the center; reset nSteps to -1 --*/
    if (n->nStepsToCenter == nnodessq) {
      n->nStepsToCenter = -1;
    } else if (n->nStepsToCenter > ggv->radial->nStepsToCenter) {
      ggv->radial->nStepsToCenter = n->nStepsToCenter;
    }
  }
}


void setNChildren (ggvisd *ggv, datad *d)
{
  gint m, i;
  noded *n;

  for (m=0; m<d->nrows_in_plot; m++) {
    i = d->rows_in_plot[m];
    n = &ggv->radial->nodes[i];

    if (n->nStepsToCenter != -1)
      if (n->parentNode != NULL)
        n->parentNode->nChildren++;
  }

/*-- debug --*/
/*
  for (i=0; i<d->nrows_in_plot; i++) {
    n = &ggv->radial->nodes[i];
    g_printerr ("node %d children %d\n", n->i, n->nChildren);
  }
*/
}

/*
 * This is currently being computed three times; once ought
 * to be enough.
*/
static void
childNodes (GList **children, noded *n) {
  GList *l;
  noded *n1;
  GList *connectedNodes = list_subset_uniq (n->connectedNodes);

  for (l = connectedNodes; l; l = l->next) {
    n1 = (noded *) l->data;

    if (n1->parentNode != NULL && n1->parentNode->i == n->i)
      if (g_list_index (*children, n1) == -1)
        *children = g_list_append (*children, n1);
  }

  list_clear (connectedNodes);
}

/*
 * Once the parent node is irrevocably set (once setParentNodes and
 * setNChildren are through), then it's possible to compute subtreeSize
 * for each node.
 *
 * Work out from the center ...
*/
gint
setSubtreeSize (noded *n, ggvisd *ggv, datad *d) {
  noded *nchild;
  GList *l, *children = NULL;

  childNodes (&children, n);

  for (l = children; l; l = l->next) {
    nchild = (noded *) l->data;

    if (!d->sampled.els[ nchild->i ] || d->hidden.els[ nchild->i ])
      continue;
    if (nchild->nStepsToCenter == -1)
      continue;

    if (nchild->nChildren == 0)
      n->subtreeSize += 1;
    else
      n->subtreeSize += setSubtreeSize (nchild, ggv, d);
  }
  /*g_printerr ("node %d subtreeSize %d\n", n->i, n->subtreeSize);*/
  return (n->subtreeSize);
}

/*---------------------------------------------------------------------*/

static void
setChildSubtreeSpans (noded *n, ggvisd *ggv, datad *d)
{
  noded *nchild;
  GList *l, *children = NULL;

  childNodes (&children, n);

  for (l = children; l; l = l->next) {
    nchild = (noded *) l->data;

    if (!d->sampled.els[ nchild->i ] || d->hidden.els[ nchild->i ])
      continue;
    if (nchild->nStepsToCenter == -1)
      continue;

    nchild->span = n->span * nchild->subtreeSize / n->subtreeSize;
    if (nchild->nChildren > 0) {
      setChildSubtreeSpans (nchild, ggv, d);
    }
  }
}


void
setSubtreeSpans (ggvisd *ggv, datad *d) {
  ggv->radial->centerNode->span = 2*M_PI;
  setChildSubtreeSpans (ggv->radial->centerNode, ggv, d);
}

/*---------------------------------------------------------------------*/

  // Set the node positions for the 2nd and later rings.
static void
setChildNodePositions (noded *n, ggvisd *ggv, datad *d)
{
  gint i;
  noded *nchild;
  gdouble theta;
  GList *l, *children = NULL;

  // the initial value of theta is the angle of the boundary of the fan
  if (n->i == ggv->radial->centerNode->i) theta = 0;
  else if (n->nChildren == 1) theta = n->theta;
  else {
    theta = n->theta - n->span/2;
  }

  // Build an array of the child nodes 
  childNodes (&children, n);

  i = 0;
  for (l = children; l; l = l->next) {
    nchild = (noded *) l->data;

    if (!d->sampled.els[ nchild->i ] || d->hidden.els[ nchild->i ])
      continue;
    if (nchild->nStepsToCenter == -1)
      continue;

    if (i == 0) {
      nchild->theta = theta;

      if (nchild->span > 0)
        theta += nchild->span/2;
      else  // if it's a leaf node
        theta += .5 * (n->span)/(gdouble)(n->subtreeSize-1);

      i++;
    } else {
      nchild->theta = theta + nchild->span/2;

      if (nchild->span > 0)
        theta += nchild->span;
      else  // if it's a leaf node
        theta += (n->span)/(gdouble)(n->subtreeSize-1);
    }
      
    nchild->pos.x = nchild->nStepsToCenter * cos(nchild->theta);
    nchild->pos.y = nchild->nStepsToCenter * sin(nchild->theta);

    if (nchild->nChildren > 0)
      setChildNodePositions(nchild, ggv, d);
  }
}


void
setNodePositions (ggvisd *ggv, datad *d) {

  // Set the position of the center node
  ggv->radial->centerNode->pos.x = 0;
  ggv->radial->centerNode->pos.y = 0;
  ggv->radial->centerNode->theta = 0;

  setChildNodePositions (ggv->radial->centerNode, ggv, d);
}

