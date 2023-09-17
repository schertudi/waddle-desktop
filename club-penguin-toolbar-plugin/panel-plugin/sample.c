/*  $Id$
 *
 *  Copyright (C) 2019 John Doo <john@foo.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 * 
 * config saved at .config/xfce4/panel/sample-13.rc
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <libxfce4util/libxfce4util.h>
#include <libxfce4panel/libxfce4panel.h>
#include <cairo.h>

#include <math.h>
#include "sample.h"
#include "sample-dialogs.h"

/* default settings */
#define DEFAULT_SETTING1 NULL
#define DEFAULT_SETTING2 1
#define DEFAULT_SETTING3 FALSE

#define LAUNCHER_1_TOOLTIP "Settings"
#define LAUNCHER_1_COMMAND "xfce4-settings-manager"

#define LAUNCHER_2_TOOLTIP "Terminal"
#define LAUNCHER_2_COMMAND "xfce4-terminal"

#define LAUNCHER_3_TOOLTIP "Browse Files"
#define LAUNCHER_3_COMMAND "thunar"

#define LAUNCHER_4_TOOLTIP "System Monitor"
#define LAUNCHER_4_COMMAND "ksysguard"

typedef struct {
    GtkWidget* normalImage;
    GtkWidget* hoverImage;
    GtkWidget* tooltipWindow;
    char* command;
} Launcher;


/* prototypes */
static void
sample_construct (XfcePanelPlugin *plugin);


/* register the plugin */
XFCE_PANEL_PLUGIN_REGISTER (sample_construct);



void
sample_save (XfcePanelPlugin *plugin,
             SamplePlugin    *sample)
{
  XfceRc *rc;
  gchar  *file;

  /* get the config file location */
  file = xfce_panel_plugin_save_location (plugin, TRUE);

  if (G_UNLIKELY (file == NULL))
    {
       DBG ("Failed to open config file");
       return;
    }

  /* open the config file, read/write */
  rc = xfce_rc_simple_open (file, FALSE);
  g_free (file);

  if (G_LIKELY (rc != NULL))
    {
      /* save the settings */
      DBG(".");
      if (sample->setting1)
        xfce_rc_write_entry    (rc, "setting1", sample->setting1);

      xfce_rc_write_int_entry  (rc, "setting2", sample->setting2);
      xfce_rc_write_bool_entry (rc, "setting3", sample->setting3);

      /* close the rc file */
      xfce_rc_close (rc);
    }
}



static void
sample_read (SamplePlugin *sample)
{
  XfceRc      *rc;
  gchar       *file;
  const gchar *value;

  /* get the plugin config file location */
  file = xfce_panel_plugin_save_location (sample->plugin, TRUE);
  DBG("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!LOAD PLUGIN PROPERTIES!!!");
  DBG("save file is at %s", file);

  if (G_LIKELY (file != NULL))
    {
      /* open the config file, readonly */
      rc = xfce_rc_simple_open (file, TRUE);

      /* cleanup */
      g_free (file);

      if (G_LIKELY (rc != NULL))
        {
          /* read the settings 
          value = xfce_rc_read_entry (rc, "setting1", DEFAULT_SETTING1);
          sample->setting1 = g_strdup (value);

          sample->setting2 = xfce_rc_read_int_entry (rc, "setting2", DEFAULT_SETTING2);
          sample->setting3 = xfce_rc_read_bool_entry (rc, "setting3", DEFAULT_SETTING3);
          */

          value = xfce_rc_read_entry (rc, "launcher-1-tooltip", LAUNCHER_1_TOOLTIP);
          sample->launcher1Tooltip = g_strdup (value);
          value = xfce_rc_read_entry (rc, "launcher-1-command", LAUNCHER_1_COMMAND);
          sample->launcher1Command = g_strdup (value);

          value = xfce_rc_read_entry (rc, "launcher-2-tooltip", LAUNCHER_2_TOOLTIP);
          sample->launcher2Tooltip = g_strdup (value);
          value = xfce_rc_read_entry (rc, "launcher-2-command", LAUNCHER_2_COMMAND);
          sample->launcher2Command = g_strdup (value);

          value = xfce_rc_read_entry (rc, "launcher-3-tooltip", LAUNCHER_3_TOOLTIP);
          sample->launcher3Tooltip = g_strdup (value);
          value = xfce_rc_read_entry (rc, "launcher-3-command", LAUNCHER_3_COMMAND);
          sample->launcher3Command = g_strdup (value);

          value = xfce_rc_read_entry (rc, "launcher-4-tooltip", LAUNCHER_4_TOOLTIP);
          sample->launcher4Tooltip = g_strdup (value);
          value = xfce_rc_read_entry (rc, "launcher-4-command", LAUNCHER_4_COMMAND);
          sample->launcher4Command = g_strdup (value);

          /* cleanup */
          xfce_rc_close (rc);

          /* leave the function, everything went well */
          return;
        }
    }

  /* something went wrong, apply default values */
  DBG ("Applying default settings");

  sample->setting1 = g_strdup (DEFAULT_SETTING1);
  sample->setting2 = DEFAULT_SETTING2;
  sample->setting3 = DEFAULT_SETTING3;

  sample->launcher1Tooltip = LAUNCHER_1_TOOLTIP;
  sample->launcher1Command = LAUNCHER_1_COMMAND;

  sample->launcher2Tooltip = LAUNCHER_2_TOOLTIP;
  sample->launcher2Command = LAUNCHER_2_COMMAND;

  sample->launcher3Tooltip = LAUNCHER_3_TOOLTIP;
  sample->launcher3Command = LAUNCHER_3_COMMAND;

  sample->launcher4Tooltip = LAUNCHER_4_TOOLTIP;
  sample->launcher4Command = LAUNCHER_4_COMMAND;
}


void click (GtkButton* button)
{

}



static void do_drawing(cairo_t *,  int allocWidth, int allocHeight, SamplePlugin *sample);

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, SamplePlugin *sample)
{      
   //GdkWindow* win = gtk_widget_get_parent_window(widget);
    //GdkCursor* watchCursor = gdk_cursor_new(GDK_XTERM);
    //gdk_window_set_cursor(win, watchCursor);
  
  GtkAllocation alloc;
  gtk_widget_get_allocation (GTK_WIDGET (widget), &alloc);


  do_drawing(cr, alloc.width, alloc.height, sample);
  DBG ("draw event %d %d", alloc.width, alloc.height);

  return FALSE;
}

static cairo_surface_t* load_image(char *folderPath, char *fileName) {
  char path[PATH_MAX];
  sprintf(path, "%s%s", folderPath, fileName);
  cairo_surface_t *surface = cairo_image_surface_create_from_png(path);
  return surface;
}

static void do_drawing(cairo_t *cr, int allocWidth, int allocHeight, SamplePlugin *sample)
{
  //LOAD IMAGES
  /*
  cairo_surface_t *left = load_image(path, "bar-left.png");
  cairo_surface_t *right = load_image(path, "bar-right.png");
  cairo_surface_t *top = load_image(path, "bar-top.png");
  cairo_surface_t *bottom = load_image(path, "bar-bottom.png");
  */
 cairo_surface_t *left = sample->searchImgLeft;
  cairo_surface_t *right = sample->searchImgRight;
  cairo_surface_t *top = sample->searchImgTop;
  cairo_surface_t *bottom = sample->searchImgBottom;

  int x, y;


  //PAINT BACKGROUND
  cairo_surface_t *img = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 1, 1);
  cairo_set_source_rgba(cr, 0, 0, 0, 0); 
  cairo_move_to(cr, 100, 100);
  cairo_paint(cr);  

  int padding = 0;

  cairo_set_source_rgba(cr, 0, 0.2, 0.40392156862, 1);  // #003367
  x = cairo_image_surface_get_width(left) - padding;
  y = 0;
  int width = allocWidth - cairo_image_surface_get_width(left) - cairo_image_surface_get_width(right);
  int height = cairo_image_surface_get_height(left);
  cairo_rectangle(cr, x, y, x + width, y + height);
  cairo_fill(cr);

  padding = 5;


  //PAINT TOP
  
  x = cairo_image_surface_get_width(left) - padding;
  y = 0;
  float topWidth = allocWidth - cairo_image_surface_get_width(left) - cairo_image_surface_get_width(right) + padding * 2;
  float topHeight = cairo_image_surface_get_height(top);

  cairo_pattern_t *pattern = cairo_pattern_create_for_surface(top);
  cairo_pattern_set_filter(pattern, CAIRO_FILTER_NEAREST);

  cairo_identity_matrix(cr);
  cairo_translate(cr, x, y);
  cairo_set_source_surface(cr, top, 0, 0);
  cairo_scale(cr, topWidth / cairo_image_surface_get_width(top), topHeight / cairo_image_surface_get_height(top));
  
  cairo_set_source(cr, pattern);
  cairo_paint(cr);

  //PAINT BOTTOM
  
  x = cairo_image_surface_get_width(left) - padding;
  y = 40;

  cairo_identity_matrix(cr);
  cairo_translate(cr, x, y);
  cairo_set_source_surface(cr, bottom, 0, 0);
  cairo_scale(cr, topWidth / cairo_image_surface_get_width(bottom), 10 / cairo_image_surface_get_height(bottom));
  
  pattern = cairo_pattern_create_for_surface(bottom);
  cairo_pattern_set_filter(pattern, CAIRO_FILTER_NEAREST);
  cairo_set_source(cr, pattern);
  cairo_paint(cr); 

    //PAINT LEFT
    cairo_identity_matrix(cr);
  cairo_set_source_surface(cr, left, 0, 0);
  cairo_paint(cr);  

  //PAINT RIGHT
  
  cairo_identity_matrix(cr);
  cairo_set_source_surface(cr, right, cairo_image_surface_get_width(left) + topWidth - padding * 2, 0);
  cairo_paint(cr); 

  //CLEANUP
  //cairo_surface_flush(img);
  cairo_pattern_destroy(pattern);
}

static gboolean  button_press_event (
  GtkWidget* self,
  GdkEventButton* event,
  SamplePlugin *sample
) {
  

  gint x, y, width, height; 
  g_autoptr(GError) error = NULL;
  GtkAllocation allocation;
  //const char *command;
  char command[PATH_MAX];

  printf("CLICKED PANEL \n");

  gdk_window_get_origin (gtk_widget_get_window (GTK_WIDGET (self)), &x, &y);
  gtk_widget_get_allocation(self, &allocation); //use self instead of dareax
  width = allocation.width - 15;
  height = allocation.height;
  height = 250;
  y -= 250;
  y += allocation.height;
  printf ("main window width = %d, height = %d, x = %d y = %d\n", width, height, x, y);

  snprintf(command, PATH_MAX,
    "rofi -show drun -theme-str \
      'configuration {location: 7; xoffset: %d;} \
      window {width: %dpx;}'",
      x, width);

  g_spawn_command_line_async(command, error);
  if (error != NULL) {
    g_error ("Spawning child failed: %s", error->message);
  }
  printf("started rofi\n");

  return FALSE;

  
}

/*
nah 

static void show_info_window() {
  GtkWidget *window;

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), "System Information");
  gtk_window_set_default_size (GTK_WINDOW (window), 500, 500);
  gtk_widget_show (window);

  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_widget_show (box);
  gtk_container_add (GTK_CONTAINER (window), box);

  GtkWidget *time = gtk_label_new("It is currently 12:00 AM");
  GtkWidget *uptime = gtk_label_new("Your computer has been on for 1000 hours");
  GtkWidget *volume = gtk_label_new("Volume: 100%");
  GtkWidget *battery = gtk_label_new("Battery: 100%");

  gtk_widget_show (time);
  gtk_widget_show (uptime);

  gtk_box_pack_start (GTK_BOX (box), time, False, False, 5);
  gtk_box_pack_start (GTK_BOX (box), uptime, False, False, 5);
}
*/

static gboolean  launcher_press (
  GtkWidget* self,
  Launcher* launch
) {
  DBG("launcher clicked so run command %s", launch->command);

/*
  if (launch->command == "info") {
    DBG("launch info panel");
    show_info_window();
    return False;
  }
  */

  g_autoptr(GError) error = NULL;
  g_spawn_command_line_async(launch->command, error);
  if (error != NULL) {
    g_error ("Spawning child failed: %s", error->message);
  }

  return False;
}



static gboolean  launcher_release (
  GtkWidget* self,
  GdkEventButton* event,
  gpointer user_data
) {
  DBG("launcher released");
  return False;
}

static gboolean
launcher_enter (
  GtkWidget* self,
  GdkEventCrossing* event,
  Launcher* launch
) {
  gint wx, wy;
  gdk_window_get_origin (gtk_widget_get_window (launch->normalImage), &wx, &wy);
  GtkRequisition winSize;
  gtk_window_get_size(launch->tooltipWindow, &winSize, NULL);
  int buttonSize = gtk_widget_get_allocated_width(launch->normalImage);

  wx -= winSize.width / 2 - buttonSize / 2; //center it along the button
  wy -= 45; //move up to appear over bar

  gtk_window_move(launch->tooltipWindow, wx, wy);
  gtk_widget_show(launch->tooltipWindow);
  return False;
}

static gboolean
launcher_exit (
  GtkWidget* self,
  GdkEventCrossing* event,
  Launcher* launch
) {
  gtk_widget_hide(launch->tooltipWindow);
  //gtk_widget_hide(launch->hoverImage);
  //gtk_widget_show(launch->normalImage);
  return False;
}

static void make_launcher(char* buttonClass, char* tooltipText, char* command, GtkWidget *parent) {


  GtkWidget *label = gtk_label_new(tooltipText);
  gtk_widget_show(label);
  gtk_misc_set_alignment(GTK_MISC(label), 0.5f, 0.5f);

  GtkWidget *window;
window = gtk_window_new (GTK_WINDOW_POPUP);
gtk_window_set_decorated (GTK_WINDOW (window), FALSE);
gtk_window_set_default_size (GTK_WINDOW (window), 10, 10);
gtk_widget_hide (window);


GtkWidget *frame;

frame = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 2);
gtk_widget_show (frame);

gchar *home = g_get_home_dir();
char path[PATH_MAX];
sprintf(path, "%s/.local/share/waddleos/panel-assets/style.css", home);



GtkCssProvider *prov = gtk_css_provider_new();
//gtk_css_provider_load_from_data(prov, ".MyFrame{border:3px solid #8e7318; background-color: #feef7e;}", -1, NULL);
gtk_css_provider_load_from_file (prov, g_file_new_for_path (path), NULL);


GtkStyleContext *ctx = gtk_widget_get_style_context (frame);
gtk_style_context_add_class (ctx, "ClubPenguinTooltip");
gtk_style_context_add_provider (ctx, GTK_STYLE_PROVIDER (prov), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

  gtk_container_add(GTK_CONTAINER (window), frame);
  //gtk_container_add(GTK_CONTAINER (frame), label);

  gtk_widget_set_halign(label, GTK_ALIGN_CENTER);
  gtk_widget_set_valign(label, GTK_ALIGN_CENTER);
  gtk_box_pack_start(GTK_BOX(frame), label, TRUE, TRUE, 0);



GtkWidget *button;

// Create the button widget
button = gtk_button_new();
//button = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 2);

GtkCssProvider *provider = gtk_css_provider_new ();
gtk_css_provider_load_from_file (provider, g_file_new_for_path (path), NULL);


GtkStyleContext *context = gtk_widget_get_style_context (button);
gtk_style_context_add_class (context, "LauncherButton");
gtk_style_context_add_class (context, buttonClass);
gtk_style_context_add_provider (context, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

// Add the button widget to the window
gtk_container_add (GTK_CONTAINER (window), button);

gtk_widget_show(button);



  GtkWidget  *event = gtk_event_box_new ();
  gtk_widget_show(event);
  gtk_widget_set_events(event, GDK_BUTTON_PRESS_MASK);

  GtkWidget *box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 2);
  gtk_widget_show(box);

   gtk_container_add(GTK_CONTAINER (parent), event);
  gtk_container_add(GTK_CONTAINER (event), button);
  //gtk_container_add(GTK_CONTAINER (box), button);

  Launcher launch;
  launch.normalImage = button;
  launch.command = command;
  launch.tooltipWindow = window;

  Launcher *launchPtr = g_malloc(sizeof(Launcher)); //TODO free
  *launchPtr = launch;

  DBG("events %d", gtk_widget_get_events(event));

  
  g_signal_connect (G_OBJECT (event), "enter-notify-event", G_CALLBACK (launcher_enter), launchPtr);

  g_signal_connect (G_OBJECT (event), "leave-notify-event", G_CALLBACK (launcher_exit), launchPtr);

  g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (launcher_press), launchPtr);

  //g_signal_connect (G_OBJECT (event), "button-release-event", G_CALLBACK (launcher_release), launchPtr);
}

gboolean supports_alpha = FALSE;
static void screen_changed(GtkWidget *widget, GdkScreen *old_screen, gpointer userdata)
{
  DBG("screen changed");
    /* To check if the display supports alpha channels, get the visual */
    GdkScreen *screen = gtk_widget_get_screen(widget);
    GdkVisual *visual = gdk_screen_get_rgba_visual(screen);

    if (!visual)
    {
        printf("Your screen does not support alpha channels!\n");
        visual = gdk_screen_get_system_visual(screen);
        supports_alpha = FALSE;
    }
    else
    {
        printf("Your screen supports alpha channels!\n");
        supports_alpha = TRUE;
    }

    gtk_widget_set_visual(widget, visual);
}

static gboolean draw2(GtkWidget *widget, cairo_t *cr, gpointer userdata)
{
    cairo_save (cr);

    if (supports_alpha)
    {
        cairo_set_source_rgba (cr, 0.5, 1.0, 0.50, 0); /* transparent */
    }
    else
    {
        cairo_set_source_rgb (cr, 1.0, 1.0, 1.0); /* opaque white */
    }

    /* draw the background */
    cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
    cairo_paint (cr);

    cairo_restore (cr);

    return FALSE;
}

static void on_screen_changed(GdkScreen *screen, gpointer user_data)
{
    // screen resolution has changed
}

static void on_configure_event(GtkWidget *widget, GdkEventConfigure *event, gpointer data)
{
    int width = event->width;
    int height = event->height;

    g_print("Screen resolution changed: %dx%d\n", width, height);
}

static gboolean
searchbar_enter (
  GtkWidget* self,
  GdkEventCrossing* event,
  Launcher* launch
) {
  DBG("enter");
     GdkWindow* win = gtk_widget_get_parent_window(self);
    GdkCursor* watchCursor = gdk_cursor_new(GDK_XTERM);
    gdk_window_set_cursor(win, watchCursor);
  return False;
}

static gboolean
searchbar_exit (
  GtkWidget* self,
  GdkEventCrossing* event,
  Launcher* launch
) {
  DBG("exit");
     GdkWindow* win = gtk_widget_get_parent_window(self);
    GdkCursor* watchCursor = gdk_cursor_new(GDK_LEFT_PTR);
    gdk_window_set_cursor(win, watchCursor);
  return False;
}

static SamplePlugin *
sample_new (XfcePanelPlugin *plugin)
{
  SamplePlugin   *sample;
  GtkOrientation  orientation;
  GtkWidget      *label;
  GtkCssProvider *cssProvider;
  GtkStyleContext *context;
  GtkWidget *leftImage;
  GtkWidget *rightImage;

  /* allocate memory for the plugin structure */
  sample = g_slice_new0 (SamplePlugin);

  /* pointer to plugin */
  sample->plugin = plugin;

  /* read the user settings */
  sample_read (sample);

  /* get the current orientation */
  orientation = xfce_panel_plugin_get_orientation (plugin);

  

  /* create some panel widgets */
  sample->ebox = gtk_event_box_new ();
  gtk_widget_show (sample->ebox);

  

  sample->hvbox = gtk_box_new (orientation, 2);
  gtk_widget_show (sample->hvbox);
  gtk_container_add (GTK_CONTAINER (sample->ebox), sample->hvbox);

 
 GtkWidget *darea  = gtk_drawing_area_new();
 gtk_widget_show(darea);
 
 gtk_widget_set_size_request (darea, 200, 40);
 gtk_widget_set_hexpand(darea, TRUE);

GtkWidget       *textEvent = gtk_event_box_new ();
 g_signal_connect (G_OBJECT (textEvent), "button-press-event", G_CALLBACK (button_press_event), sample);
   g_signal_connect (G_OBJECT (textEvent), "enter-notify-event", G_CALLBACK (searchbar_enter), NULL);
  g_signal_connect (G_OBJECT (textEvent), "leave-notify-event", G_CALLBACK (searchbar_exit), NULL);

  gtk_widget_show (textEvent);

  gtk_container_add(GTK_CONTAINER (sample->hvbox), textEvent);
  gtk_container_add(GTK_CONTAINER (textEvent), darea);
  
    

 sample->darea = darea;

   gchar *home = g_get_home_dir();
  DBG("HOME DIR: %s", home);

  char path[PATH_MAX];
  sprintf(path, "%s/.local/share/waddleos/panel-assets/", home);
  DBG("FULL PATH: %s", path);

  sample->searchImgLeft = load_image(path, "bar-left.png");
  sample->searchImgRight = load_image(path, "bar-right.png");
  sample->searchImgTop = load_image(path, "bar-top.png");
  sample->searchImgBottom = load_image(path, "bar-bottom.png");





 g_signal_connect(G_OBJECT(darea), "draw", G_CALLBACK(on_draw_event), sample); 

  make_launcher("Launcher1", sample->launcher1Tooltip, sample->launcher1Command, sample->hvbox);
  make_launcher("Launcher2", sample->launcher2Tooltip, sample->launcher2Command,   sample->hvbox);
  make_launcher("Launcher3", sample->launcher3Tooltip, sample->launcher3Command,   sample->hvbox);
  make_launcher("Launcher4", sample->launcher4Tooltip, sample->launcher4Command,   sample->hvbox);

  int bar_width = 1253;
  int bar_height = 71;
  int y_offset = 5;

  GtkWidget *window;
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    //gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_decorated (GTK_WINDOW (window), FALSE);
     gtk_window_move(window, gdk_screen_width() / 2 - bar_width / 2, gdk_screen_height() - bar_height + y_offset);
    gtk_window_set_default_size(GTK_WINDOW(window), bar_width, bar_height);
    g_signal_connect(G_OBJECT(window), "delete-event", gtk_main_quit, NULL);

    gtk_widget_set_app_paintable(window, TRUE);

    gtk_window_set_skip_taskbar_hint(GTK_WINDOW (window), True);
    gtk_window_set_keep_below(window, True);
    gtk_window_set_type_hint(window, GDK_WINDOW_TYPE_HINT_DOCK);


    g_signal_connect(G_OBJECT(window), "draw", G_CALLBACK(draw2), NULL);
    g_signal_connect(G_OBJECT(window), "screen-changed", G_CALLBACK(screen_changed), NULL);
  screen_changed(window, NULL, NULL);
  //g_signal_connect(window, "configure-event", G_CALLBACK(on_configure_event), NULL);
  //GdkScreen *screen = gtk_widget_get_screen(window);
  //g_signal_connect(G_OBJECT(screen), "size-changed", G_CALLBACK(on_screen_changed), NULL);

//size-changed

gtk_widget_show (window);


  char backgroundPath[PATH_MAX];
  sprintf(backgroundPath, "%sbar-background.png", path);
  DBG("BACKGROUND PATH: %s", backgroundPath);

 GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(backgroundPath, NULL);
 gdk_pixbuf_add_alpha(pixbuf, TRUE, 0, 0, 0);
  GtkWidget *image = gtk_image_new_from_pixbuf(pixbuf);
 gtk_widget_show (image);
 gtk_container_add(GTK_CONTAINER (window), image);
 //gtk_widget_set_visual(image, visual);
 gtk_widget_set_app_paintable (image, TRUE); // important or you will get solid color
 
 return sample;
}





static void
sample_free (XfcePanelPlugin *plugin,
             SamplePlugin    *sample)
{
  GtkWidget *dialog;

  /* check if the dialog is still open. if so, destroy it */
  dialog = g_object_get_data (G_OBJECT (plugin), "dialog");
  if (G_UNLIKELY (dialog != NULL))
    gtk_widget_destroy (dialog);

  /* destroy the panel widgets */
  gtk_widget_destroy (sample->hvbox);

  /* cleanup the settings */
  if (G_LIKELY (sample->setting1 != NULL))
    g_free (sample->setting1);

  /* free the plugin structure */
  g_slice_free (SamplePlugin, sample);
}



static void
sample_orientation_changed (XfcePanelPlugin *plugin,
                            GtkOrientation   orientation,
                            SamplePlugin    *sample)
{
  /* change the orientation of the box */
  gtk_orientable_set_orientation(GTK_ORIENTABLE(sample->hvbox), orientation);
}



static gboolean
sample_size_changed (XfcePanelPlugin *plugin,
                     gint             size,
                     SamplePlugin    *sample)
{
  GtkOrientation orientation;
  GtkAllocation     alloc;
  
  /* get the orientation of the plugin */
  orientation = xfce_panel_plugin_get_orientation (plugin);

  /* set the widget size */
  if (orientation == GTK_ORIENTATION_HORIZONTAL)
    gtk_widget_set_size_request (GTK_WIDGET (plugin), -1, size);
  else
    gtk_widget_set_size_request (GTK_WIDGET (plugin), size, -1);

  /* we handled the orientation */



  return TRUE;
}

static void getsize_main_window(GtkWidget *widget, GtkAllocation *allocation, SamplePlugin *sample)
{
  gint wx, wy; 
  FILE *fptr;
  const char *homedir;
  char pathbuf[PATH_MAX];
  gchar      *launch[] = { "rofi", "-show drun --normal-window", NULL };
  gint child_stdout, child_stderr;
  

  gdk_window_get_origin (gtk_widget_get_window (widget), &wx, &wy);
  printf ("main window width = %d, height = %d, x = %d y = %d\n", allocation->width, allocation->height, wx, wy);



}




static void
sample_construct (XfcePanelPlugin *plugin)
{
  SamplePlugin *sample;

  /* setup transation domain */
  xfce_textdomain(GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR, "UTF-8");

  /* create the plugin */
  sample = sample_new (plugin);


  /* add the ebox to the panel */
  gtk_container_add (GTK_CONTAINER (plugin), sample->ebox);

  

  /* show the panel's right-click menu on this ebox */
  xfce_panel_plugin_add_action_widget (plugin, sample->ebox);

  /* connect plugin signals */
  g_signal_connect (G_OBJECT (plugin), "free-data",
                    G_CALLBACK (sample_free), sample);

  g_signal_connect (G_OBJECT (plugin), "save",
                    G_CALLBACK (sample_save), sample);

  g_signal_connect (G_OBJECT (plugin), "size-changed",
                    G_CALLBACK (sample_size_changed), sample);

  g_signal_connect (G_OBJECT (plugin), "orientation-changed",
                    G_CALLBACK (sample_orientation_changed), sample);

  /* show the configure menu item and connect signal */
  /*
  xfce_panel_plugin_menu_show_configure (plugin);
  g_signal_connect (G_OBJECT (plugin), "configure-plugin",
                    G_CALLBACK (sample_configure), sample);
                    */

  /* show the about menu item and connect signal */
  xfce_panel_plugin_menu_show_about (plugin);
  g_signal_connect (G_OBJECT (plugin), "about",
                    G_CALLBACK (sample_about), NULL);

  xfce_panel_plugin_set_expand(XFCE_PANEL_PLUGIN (plugin), True);

   g_signal_connect(G_OBJECT (plugin), "size-allocate",
                    G_CALLBACK(getsize_main_window), sample);

/*
   g_signal_connect (G_OBJECT (plugin), "button-press-event",
                    G_CALLBACK (button_press_event), sample);
                    */
}
