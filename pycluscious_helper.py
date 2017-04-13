from netrc import netrc
user, acct, passwd = netrc().authenticators("harris")

import matplotlib.pyplot as plt
from matplotlib.colors import Normalize
from matplotlib import ticker

from fiona.crs import from_epsg
import pysal as ps
import pandas as pd
import geopandas as gpd
import numpy as np
import psycopg2

import ps_query

# Needed to keep the centroids in the boundary.
import shapely
from shapely.wkt import loads, dumps
from shapely.geometry import MultiPolygon, Polygon, Point, MultiLineString, LineString, LinearRing, asShape

import os, glob

shapefile = "shapes/{}.shp"
stateinfo = "shapes/{}_info.csv"
edge_file = "shapes/{}_edges"
node_file = "shapes/{}_nodes"

def ens_dir(f, quiet = False):
  if not os.path.isdir(f):
    os.makedirs(f)
    print("Remade file", f)

def ens_data(usps): 

  ens_dir("shapes/")
  cache_stateinfo(usps)
  cache_shapefile(usps)
  cache_edge_file(usps)
  cache_node_file(usps)


import pycluscious as pycl
pycl_methods = {"reock"     : pycl.ObjectiveMethod.REOCK, 
                "dist_a"    : pycl.ObjectiveMethod.DISTANCE_A,
                "dist_p"    : pycl.ObjectiveMethod.DISTANCE_P,
                "inertia_a" : pycl.ObjectiveMethod.INERTIA_A,
                "inertia_p" : pycl.ObjectiveMethod.INERTIA_P,
                "polsby"    : pycl.ObjectiveMethod.POLSBY,
                "hull_a"    : pycl.ObjectiveMethod.HULL_A,
                "path_frac" : pycl.ObjectiveMethod.PATH_FRAC,
                "ehrenburg" : pycl.ObjectiveMethod.EHRENBURG}


pycl_circles = {"area"     : pycl.RadiusType.EQUAL_AREA, 
                "area_pop" : pycl.RadiusType.EQUAL_AREA_POP, 
                "circ"     : pycl.RadiusType.EQUAL_CIRCUMFERENCE, 
                "scc"      : pycl.RadiusType.SCC, 
                "lic"      : pycl.RadiusType.LIC}


us_states = ["al", "ak", "az", "ar", "ca", "co", "ct", "de", "dc",
             "fl", "ga", "hi", "id", "il", "in", "ia", "ks", "ky",
             "la", "me", "md", "ma", "mi", "mn", "ms", "mo", "mt",
             "ne", "nv", "nh", "nj", "nm", "ny", "nc", "nd", "oh",
             "ok", "or", "pa", "ri", "sc", "sd", "tn", "tx", "ut",
             "vt", "va", "wa", "wv", "wi", "wy"]


def get_epsg (usps): return get_state_info(usps)["epsg"]
def get_fips (usps): return get_state_info(usps)["fips"]
def get_seats(usps): return get_state_info(usps)["seats"]

def get_state_info(usps):
   
   return pd.read_csv(stateinfo.format(usps)).ix[0]


def cache_stateinfo(usps, filename = None):

   if not filename:
      filename = stateinfo.format(usps.lower())

   if os.path.exists(filename): return

   con = psycopg2.connect(database = "census", user = user, password = passwd,
                          host = "saxon.harris.uchicago.edu", port = 5432)

   info = pd.read_sql("SELECT epsg, fips, seats FROM states WHERE usps = upper('{}');".format(usps), con)
   info.to_csv(filename, index = False)


def cache_shapefile(usps, filename = None):

   if not filename:
      filename = shapefile.format(usps.lower())

   if os.path.exists(filename): return

   state = get_state_info(usps)

   con = psycopg2.connect(database = "census", user = user, password = passwd,
                          host = "saxon.harris.uchicago.edu", port = 5432)

   geo_df = gpd.GeoDataFrame.from_postgis(ps_query.tracts.format(state["fips"]), con,
                                          geom_col='geometry', crs = from_epsg(state["epsg"]))

   geo_df["id"] = geo_df.index
   geo_df[["id", "a", "pop", "x", "y", "split", "geometry"]].to_file(filename)



def cache_edge_file(usps, filename = None):

   if not filename:
      filename = edge_file.format(usps.lower())

   if os.path.exists(filename + ".csv"): return

   state = get_state_info(usps)

   con = psycopg2.connect(database = "census", user = user, password = passwd,
                          host = "saxon.harris.uchicago.edu", port = 5432)

   geo_df = gpd.GeoDataFrame.from_postgis(ps_query.edges.format(state["fips"]), con,
                                          geom_col='lines', crs = from_epsg(state["epsg"]))

   geo_df[geo_df["rev"] == False][["eid", "lines"]].to_file(filename + ".shp")

   geo_df[["rn","seq","eid","rev","nodea","nodeb"]].to_csv(filename + ".csv", index = False)


def cache_node_file(usps, filename = None):

   if not filename:
      filename = node_file.format(usps.lower())

   if os.path.exists(filename + ".csv"): return

   state = get_state_info(usps)

   con = psycopg2.connect(database = "census", user = user, password = passwd,
                          host = "saxon.harris.uchicago.edu", port = 5432)

   ndf = pd.read_sql(ps_query.nodes.format(state["fips"]), con)

   ndf[["nid", "x", "y", "nseq", "eid"]].to_csv(filename + ".csv", index = False)

   geometry = [Point(xy) for xy in zip(ndf.x, ndf.y)]
   geo_ndf = gpd.GeoDataFrame(ndf, crs = from_epsg(state["epsg"]), geometry=geometry)

   geo_ndf[ndf["nseq"] == 1][["nid", "geometry"]].to_file(filename + ".shp")




def plot_map(gdf, filename, crm, hlt, shading = "district", figsize = 10, label = "", ring = None, circ = None, legend = False):

    gdf["C"] = pd.Series(crm)
    gdf["H"] = 0
    gdf.loc[hlt, "H"] = 1

    if shading == "density":
      gdf["density"] = gdf["pop"]/gdf["a"]

    dis = gdf.dissolve("C", aggfunc='sum')
    dis.reset_index(inplace = True)

    target = dis["pop"].sum() / dis.shape[0]
    dis["frac"] = dis["pop"] / target

    bounds = gdf.total_bounds
    xr = bounds[2] - bounds[0]
    yr = bounds[3] - bounds[1]
    fs = (figsize * np.sqrt(xr/yr), figsize * np.sqrt(yr/xr))

    bins = min(5, dis.shape[0])
    q = ps.Quantiles(dis["frac"], k = bins)

    labels = ["≤ {:.3f}".format(q.bins[i]) for i in range(bins) ]

    dis["qyb"] = q.yb
    dis["qyb"] = pd.Series(dis["qyb"], dtype="category")
    if len(set(labels)) == len(labels):
      dis["qyb"].cat.rename_categories(labels, inplace = True)

    if shading == "target":
      ax = dis.plot(column = "qyb", alpha = 0.3, categorical = True, cmap = "Greys",
                    legend = True, figsize = fs)

      gdf[gdf["H"] == 1].plot(facecolor = "red", alpha = 0.3, linewidth = 0.05, ax = ax)

    elif shading == "density":
      ax = gdf.plot(column = "density", cmap = "gray", scheme = "quantiles", k = 9, alpha = 0.8, figsize = fs, linewidth = 0)
      dis.plot(color = "blue", alpha = 0.3, linewidth = 1, ax = ax)

    else:
      ax = dis.plot("C", alpha = 0.5, categorical = True, cmap = "nipy_spectral", linewidth = 1, legend = legend, figsize = fs)

      gdf[gdf["H"] == 1].plot(facecolor = "grey", alpha = 0.3, linewidth = 0.05, ax = ax)

    ax.set_xlim([bounds[0] - 0.1 * xr, bounds[2] + 0.1 * xr])
    ax.set_ylim([bounds[1] - 0.1 * yr, bounds[3] + 0.1 * yr])

    if label: ax.text(bounds[0] - 0.1*xr, bounds[1] - 0.1*yr, label, fontsize = 10)

    ax.set_axis_off()

    if ring is not None:
      ring["C"] = ring.index
      if shading == "density":
        ring.plot(color = "black", ax = ax, linewidth = 2.5)
        ring.plot(color = "white", ax = ax, linewidth = 0.7)
      else:
        ring.plot("C", categorical = True, cmap = "nipy_spectral",  ax = ax, linewidth = 3)
        ring.plot(color = "white", ax = ax, linewidth = 1)

    if circ is not None:
      circ["C"] = circ.index
      circ.plot(color = "white", ax = ax, linewidth = 2)


    if not filename: return ax

    ax.figure.savefig(filename, bbox_inches='tight', pad_inches=0.05)
    plt.close('all')



def fix_mp(poly):
    
    if type(poly) != shapely.geometry.multipolygon.MultiPolygonAdapter:
        return poly
      
    if poly.is_valid: return poly
    
    mp_out = MultiPolygon()
    for p in list(poly):
        mp_out |= Polygon(p.exterior.coords[:])

    for p in list(poly):
        for ir in p.interiors:
            mp_out -= Polygon(ir.coords[:])

    return mp_out
    

# using pysal and shapely; very slightly modified from the contrib:
# https://github.com/pysal/pysal/blob/master/pysal/contrib/shared_perimeter_weights.py
def spw_from_shapefile(shapefile, norm = False):
    polygons = ps.open(shapefile, 'r').read()
    spolygons = list(map(asShape,polygons))
    spolygons = [fix_mp(p) for p in spolygons]
    perimeters = [p.length if norm else 1. for p in spolygons]
    Wsrc = ps.queen_from_shapefile(shapefile)
    new_weights, edges = {}, {}
    for i in Wsrc.neighbors:
        a = spolygons[i]
        p = perimeters[i]
        new_weights[i] = [] 
        for j in Wsrc.neighbors[i]:

            intersect = a.intersection(spolygons[j])
            new_weights[i].append(intersect.length)

            # if type(intersect) is LineString:
            #   edges[i].append((j, intersect.length))
            # if type(intersect) is MultiLineString:
            #   for line in list(intersect):
            #     edges[i].append((j, line.length))

        edges[i] = sum(new_weights[i])/a.length 

    return edges, ps.W(Wsrc.neighbors, new_weights)


