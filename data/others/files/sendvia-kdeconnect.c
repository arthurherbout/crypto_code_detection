/* Copyright 2018 KDE Connect Indicator Developers
 *
 * This software is licensed under the GNU Lesser General Public License
 * (version 2.1 or later).  See the COPYING file in this distribution.
 */

//#include <config.h>
#include <string.h>
//#include <glib/gi18n-lib.h>
#include <nautilus-extension.h>
#include "sendvia-kdeconnect.h"

struct _NautilusKdecs
{
    GObject parent_instance;    
     
    GDBusConnection *conn;
    GDBusProxy *proxy;    
    GError *error;
};

static void menu_provider_iface_init (NautilusMenuProviderInterface *iface);

G_DEFINE_DYNAMIC_TYPE_EXTENDED (NautilusKdecs, nautilus_kdecs, G_TYPE_OBJECT, 0,
                                G_IMPLEMENT_INTERFACE_DYNAMIC (NAUTILUS_TYPE_MENU_PROVIDER,
                                                               menu_provider_iface_init))

static GList *
get_file_items (NautilusMenuProvider *provider,
                GtkWidget            *window,
                GList                *files)
{
    NautilusKdecs *kdecs;

    GList *items = NULL;
    NautilusMenu *sub_menu;
    NautilusMenuItem *menu, *item;    

    g_autoptr (GVariant) devices;
    GVariantIter devicesIter;        
    g_autofree gchar *key = NULL, *value = NULL;
    
    kdecs = NAUTILUS_KDECS (provider);
    
    if (files == NULL)
    {    
        return NULL;
    }
    else 
    {
        for (GList *l = files; l != NULL; l = l->next) 
        {     
            NautilusFileInfo *file = NAUTILUS_FILE_INFO (l->data);                               
                   
            if (g_strcmp0 (nautilus_file_info_get_uri_scheme (file), "file") != 0 || 
                nautilus_file_info_is_directory (file))                         
                return NULL;                
        }

        devices = get_reachable_devices (kdecs);

        if (devices == NULL ||
            g_variant_n_children (devices) <= 0)
            return NULL;
    }            

    sub_menu = nautilus_menu_new ();
    
    menu = nautilus_menu_item_new ("SendViaExtension::SendViaKDEConnect",
                                   "KDEConnect Send To",
                                   "send file(s) with kdeconnect",
                                   "kdeconnect");
        
    nautilus_menu_item_set_submenu (menu, sub_menu);

    g_variant_iter_init (&devicesIter, devices);
    while (g_variant_iter_next (&devicesIter, "{ss}", &key, &value))
    {
        g_print ("SendViaKDEConnect Info:: Phone Key '%s', Phone Name '%s'\n", key, value);                

        item = nautilus_menu_item_new (g_strconcat ("SendViaExtension::SendViaKDEConnect", 
                                                    key,
                                                    NULL),
                                       value,
                                       g_strconcat ("send file(s) to ", 
                                                    value,
                                                    NULL),                                       
                                       "kdeconnect");

        g_signal_connect (item,
                          "activate",
                          G_CALLBACK (sendto_callback),
                          provider);

        g_object_set_data_full (G_OBJECT (item),
                                "files",
                                nautilus_file_info_list_copy (files),
                                (GDestroyNotify) nautilus_file_info_list_free);
        
        g_object_set_data (G_OBJECT (item),
                           "deviceId",
                           g_strdup (key));

        g_object_set_data (G_OBJECT (item),
                           "deviceName",
                           g_strdup (value));

        nautilus_menu_append_item (sub_menu, item);               
    }    

    items = g_list_append (items, menu);

    return items;
}

static void
menu_provider_iface_init (NautilusMenuProviderInterface *iface)
{
    iface->get_file_items = get_file_items;
}

static void
nautilus_kdecs_init (NautilusKdecs *kdecs)
{   
    kdecs->error = NULL;    
    kdecs->conn = g_bus_get_sync (G_BUS_TYPE_SESSION, 
                                  NULL, 
                                  &kdecs->error);    
 
    if (process_error (kdecs->error))     
        return NULL;

    kdecs->proxy = g_dbus_proxy_new_sync (kdecs->conn,
				                          G_DBUS_PROXY_FLAGS_NONE,
				                          NULL,				
				                          "org.kde.kdeconnect",	
				                          "/modules/kdeconnect",	
				                          "org.kde.kdeconnect.daemon",	
				                          NULL,		
				                          &kdecs->error);            
 
    if (process_error (kdecs->error))     
        return NULL;
}

//TODO: Implements finalaze that is invocable by ..nautilus_kdecs_class_finalize

// static void
// nautilus_kdecs_finalize (NautilusKdecs *kdecs)
// {
//     g_object_unref (kdecs->conn); 
//     g_object_unref (kdecs->proxy);    
//     //g_free (kdecs->error);
// }

static void
nautilus_kdecs_class_init (NautilusKdecsClass *klass)
{    
}

static void
nautilus_kdecs_class_finalize (NautilusKdecsClass *klass)
{
}

void
nautilus_kdecs_load (GTypeModule *module)
{
    nautilus_kdecs_register_type (module);
}

GVariant* 
get_reachable_devices (NautilusKdecs *kdecs)
{
    gboolean onlyReachable = TRUE, onlyPaired = TRUE;

    GVariant *result = g_dbus_proxy_call_sync(kdecs->proxy,
					                          "deviceNames",
					                          g_variant_new ("(bb)", 
                                                             onlyReachable, 
                                                             onlyPaired),
					                          G_DBUS_CALL_FLAGS_NONE,
					                          350,
					                          NULL,
					                          &kdecs->error);	
	
    if (process_error (kdecs->error))     
        return NULL;
    else
        return g_variant_get_child_value (result, 0);     
}

static void
sendto_callback (NautilusMenuItem *item,
                 gpointer          user_data)
{       
    g_autoptr (GError) error  = NULL;     
    GDBusConnection *conn;
    GDBusProxy *proxy;    
   
    GList *files;      
    g_autofree gchar *device_id, *device_name;

    files = g_object_get_data (G_OBJECT (item), "files");
    device_id = g_object_get_data (G_OBJECT (item), "deviceId");    
    device_name = g_object_get_data (G_OBJECT (item), "deviceName");    
    
    conn = g_bus_get_sync (G_BUS_TYPE_SESSION, 
                           NULL, 
                           &error);    
 
    if (process_error (error))
        return;        

    proxy = g_dbus_proxy_new_sync (conn,
			    	               G_DBUS_PROXY_FLAGS_NONE,
				                   NULL,				
				                   "org.kde.kdeconnect",		
                                   g_strconcat ("/modules/kdeconnect/devices/", 
                                                device_id, 
                                                "/share",
                                                NULL),
				                   "org.kde.kdeconnect.device.share",	
				                   NULL,		
				                   &error);

    if (process_error (error))
        return;                                   

    for (GList *l = files; l != NULL; l = l->next)
    {
        g_autofree gchar *uri = NULL;

        uri = nautilus_file_info_get_uri (l->data);
        //TODO: pass proxy by reference
        sendto_proxy (device_id, uri, proxy);

        //g_string_append_printf (command, " \"%s\"", uri);
    }

    //Ver se enviar por cmd não bloqueia
    // g_spawn_command_line_async (command->str, NULL);
    g_object_unref (conn);
    g_object_unref (proxy);    
}

static void 
sendto_proxy (gchar *device,
              gchar *uri,
              GDBusProxy *proxy)
{            
    g_autoptr (GError) error = NULL;

	g_dbus_proxy_call_sync(proxy,
					       "shareUrl",
					       g_variant_new ("(s)", 
                                          uri),
					       G_DBUS_CALL_FLAGS_NONE,
					       -1,
					       NULL,
					       &error);
	
    if (!process_error (error))             
        g_print ("SendViaKDEConnect Info:: Sender '%s', File '%s'\n", device, uri); 
    else
        g_print ("SendViaKDEConnect Error:: '%s'\n", error->message); 

    g_free (device);
    g_free (uri);    
}

static gboolean 
process_error (GError *error)
{
    if (error != NULL)
    {
        g_print ("SendViaKDEConnect Error:: '%s'\n", error->message); 
        return TRUE;
    }
    else
    {
        return FALSE;
    }        
}

