#
load libfreeflow[info sharedlibextension]

clear_data
set tol 1e-7

set map_name "map_ver_frac"

pnts_read "PZab_2001.txt" "press2001" 4 3 5
# recalc to MPa
pnts_mult_real 0.1013 "press2001"

cntr_read "lines/P_2001_236_ver.txt" "P_2001_236_ver"
# recalc to MPa
cntr_mult_real 0.1013

pnts_read "Q_2001.txt" "debits2001" 4 3 5
pnts_mult_real -1 "debits2001"

curv_read "VNK1.txt" "VNK1" 2 1
curv_read "VNK2.txt" "VNK2" 2 1
curv_read "VNK3.txt" "VNK3" 2 1
area_read "ZAM1.txt" "ZAM1" 2 1
curv_read "ZAM2.txt" "ZAM2" 2 1
area_read "ZAM5.txt" "ZAM5" 2 1
curv_read "lines/frac_236_ver.txt" "frac_236_ver"

grid_get 28300 41400 30 45500 64700 30

points "press2001"
curve 21.4 "VNK1"
curve 21.4 "VNK2"
curve 21.4 "VNK3"
area undef "ZAM1"
area undef "ZAM5"
fault "ZAM2"
lcm_simple 16.4 0.5 1e-6
contour_add 1 "P_2001_236_ver"
flow_points "debits2001"
flow_curve -0.000467413 "frac_236_ver"

surfit

grid_unload
file_save "map_ver_frac.dat"

# plot resulting surface in PostScript
surf_plot "map_ver_frac.ps" $map_name