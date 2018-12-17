angle = radians(10.)
patches = radians(360.)/angle
theta = np.arange(0,radians(360.),angle)
count = [0]*patches
for i, item in enumerate(some_array_of_azimuth_directions):
    temp = int((item - item%angle)/angle)
    count[temp] += 1
width = angle * np.ones(patches)

# force square figure and square axes looks better for polar, IMO
fig = plt.figure(figsize=(8,8))
ax = fig.add_axes([0.1, 0.1, 0.8, 0.8], polar=True)

rmax = max(count) + 1

ax.set_rlim(0,rmax)
ax.set_theta_offset(np.pi/2)
ax.set_thetagrids(np.arange(0,360,10))
ax.set_theta_direction(-1)

# project strike distribution as histogram bars
bars = ax.bar(theta, count, width=width)
r_values = []
colors = []
for r,bar in zip(count, bars):
    r_values.append(r/float(max(count)))
    colors.append(cm.jet(r_values[-1], alpha=0.5))
    bar.set_facecolor(colors[-1])
    bar.set_edgecolor('grey')
    bar.set_alpha(0.5)

# Add colorbar, make sure to specify tick locations to match desired ticklabels
colorlist = []
r_values.sort()
values = []
for val in r_values:
    if val not in values:
        values.append(val*float(max(count)))

    color = cm.jet(val, alpha=0.5)
    if color not in colorlist:
        colorlist.append(color)

cpt = mpl.colors.ListedColormap(colorlist)
bounds = range(max(count)+1)
norm = mpl.colors.BoundaryNorm(values, cpt.N-1)

cax = fig.add_axes([0.97, 0.3, 0.03, 0.4])
cb = mpl.colorbar.ColorbarBase(cax, cmap=cpt,
                                     norm=norm,
                                     boundaries=bounds,
                                     # Make the length of each extension
                                     # the same as the length of the
                                     # interior colors:
                                     extendfrac='auto',
                                     ticks=[bounds[i] for i in range(0, len(bounds), 2)],
                                     #ticks=bounds,
                                     spacing='uniform')
