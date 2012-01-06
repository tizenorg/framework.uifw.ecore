#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <process.h>

#include <Evil.h>
#include <Ecore.h>

#include "Ecore_Con.h"
#include "ecore_con_private.h"

#define BUFSIZE 512


static int _ecore_con_local_init_count = 0;

int
ecore_con_local_init(void)
{
   if (++_ecore_con_local_init_count != 1)
     return _ecore_con_local_init_count;

   return _ecore_con_local_init_count;
}

int
ecore_con_local_shutdown(void)
{
   if (--_ecore_con_local_init_count != 0)
     return _ecore_con_local_init_count;

   return _ecore_con_local_init_count;
}


static Eina_Bool
_ecore_con_local_win32_server_read_client_handler(void *data, Ecore_Win32_Handler *wh)
{
  Ecore_Con_Client *cl;
  void *buf;
  DWORD n;
  Eina_Bool broken_pipe = EINA_FALSE;

  cl = (Ecore_Con_Client *)data;

  if (!ResetEvent(cl->host_server->event_read))
    return ECORE_CALLBACK_RENEW;

  buf = malloc(cl->host_server->nbr_bytes);
  if (!buf)
    return ECORE_CALLBACK_RENEW;

  if (ReadFile(cl->host_server->pipe, buf, cl->host_server->nbr_bytes, &n, NULL))
    {
       if (!cl->delete_me)
         ecore_con_event_client_data(cl, buf, cl->host_server->nbr_bytes, EINA_FALSE);
       cl->host_server->want_write = 1;
    }
  else
    {
      if (GetLastError() == ERROR_BROKEN_PIPE)
        broken_pipe = EINA_TRUE;
    }

  if (broken_pipe)
    {
#if 0
       char *msg;

       msg = evil_last_error_get();
       if (msg)
         {
            ecore_con_event_client_error(cl, msg);
            free(msg);
         }
#endif
       if (!cl->delete_me)
         ecore_con_event_client_del(cl);
       cl->dead = EINA_TRUE;
       return ECORE_CALLBACK_CANCEL;
    }

  if (cl->host_server->want_write)
    ecore_con_local_win32_client_flush(cl);

   ecore_main_win32_handler_del(wh);

  return ECORE_CALLBACK_DONE;
}

static Eina_Bool
_ecore_con_local_win32_server_peek_client_handler(void *data, Ecore_Win32_Handler *wh)
{
   Ecore_Con_Client *cl;
#if 0
   char *msg;
#endif

   cl = (Ecore_Con_Client *)data;

   if (!ResetEvent(cl->host_server->event_peek))
     return ECORE_CALLBACK_RENEW;

#if 0
   msg = evil_last_error_get();
   if (msg)
     {
        ecore_con_event_server_error(cl->host_server, msg);
        free(msg);
     }
#endif
   if (!cl->host_server->delete_me)
     ecore_con_event_server_del(cl->host_server);
   cl->host_server->dead = EINA_TRUE;
   return ECORE_CALLBACK_CANCEL;

   ecore_main_win32_handler_del(wh);

   return ECORE_CALLBACK_DONE;
}

static Eina_Bool
_ecore_con_local_win32_client_peek_server_handler(void *data, Ecore_Win32_Handler *wh)
{
   Ecore_Con_Server *svr;
#if 0
   char *msg;
#endif

   svr = (Ecore_Con_Server *)data;

   if (!ResetEvent(svr->event_peek))
     return ECORE_CALLBACK_RENEW;
#if 0
   msg = evil_last_error_get();
   if (msg)
     {
        ecore_con_event_server_error(svr, msg);
        free(msg);
     }
#endif
   if (!svr->delete_me)
     ecore_con_event_server_del(svr);
   svr->dead = EINA_TRUE;
   return ECORE_CALLBACK_CANCEL;

   ecore_main_win32_handler_del(wh);

   return ECORE_CALLBACK_DONE;
}

static Eina_Bool
_ecore_con_local_win32_client_read_server_handler(void *data, Ecore_Win32_Handler *wh)
{
  Ecore_Con_Server *svr;
  void *buf;
  DWORD n;
  Eina_Bool broken_pipe = EINA_FALSE;

  svr = (Ecore_Con_Server *)data;

  if (!ResetEvent(svr->event_read))
    return ECORE_CALLBACK_RENEW;

  buf = malloc(svr->nbr_bytes);
  if (!buf)
    return ECORE_CALLBACK_RENEW;

  if (ReadFile(svr->pipe, buf, svr->nbr_bytes, &n, NULL))
    {
       if (!svr->delete_me)
         ecore_con_event_server_data(svr, buf, svr->nbr_bytes, EINA_FALSE);
       svr->want_write = 1;
    }
  else
    {
      if (GetLastError() == ERROR_BROKEN_PIPE)
        broken_pipe = EINA_TRUE;
    }

  if (broken_pipe)
    {
#if 0
       char *msg;

       msg = evil_last_error_get();
       if (msg)
         {
            ecore_con_event_server_error(svr, msg);
            free(msg);
         }
#endif
       if (!svr->delete_me)
         ecore_con_event_server_del(svr);
       svr->dead = EINA_TRUE;
       return ECORE_CALLBACK_CANCEL;
    }

  if (svr->want_write)
    ecore_con_local_win32_server_flush(svr);

   ecore_main_win32_handler_del(wh);

  return ECORE_CALLBACK_DONE;
}

/* thread to read data sent by the server to the client */
static unsigned int __stdcall
_ecore_con_local_win32_client_read_server_thread(void *data)
{
   Ecore_Con_Server *svr;
   DWORD nbr_bytes = 0;

   svr = (Ecore_Con_Server *)data;

   svr->read_stopped = EINA_FALSE;

   while (!svr->read_stop)
     {
        if (PeekNamedPipe(svr->pipe, NULL, 0, NULL, &nbr_bytes, NULL))
          {
             if (nbr_bytes <= 0)
               continue;

             svr->nbr_bytes = nbr_bytes;
             if (!SetEvent(svr->event_read))
               continue;
          }
        else
          {
             if (GetLastError() == ERROR_BROKEN_PIPE)
               {
                  if (!SetEvent(svr->event_peek))
                    continue;
                  break;
               }
          }
     }

   printf(" ### %s\n", __FUNCTION__);
   svr->read_stopped = EINA_TRUE;
   _endthreadex(0);
   return 0;
}

/* thread to read data sent by the client to the server */
static unsigned int __stdcall
_ecore_con_local_win32_server_read_client_thread(void *data)
{
   Ecore_Con_Client *cl;
   DWORD nbr_bytes = 0;

   cl = (Ecore_Con_Client *)data;

   cl->host_server->read_stopped = EINA_FALSE;

   while (!cl->host_server->read_stop)
     {
        if (PeekNamedPipe(cl->host_server->pipe, NULL, 0, NULL, &nbr_bytes, NULL))
          {
             if (nbr_bytes <= 0)
               continue;

             cl->host_server->nbr_bytes = nbr_bytes;
             if (!SetEvent(cl->host_server->event_read))
               continue;
          }
        else
          {
             if (GetLastError() == ERROR_BROKEN_PIPE)
               {
                  if (!SetEvent(cl->host_server->event_peek))
                    continue;
                  break;
               }
          }
     }

   printf(" ### %s\n", __FUNCTION__);
   cl->host_server->read_stopped = EINA_TRUE;
   _endthreadex(0);
   return 0;
}

static Eina_Bool
_ecore_con_local_win32_client_add(void *data, Ecore_Win32_Handler *wh)
{
   Ecore_Con_Client *cl = NULL;
   Ecore_Con_Server *svr;
   Ecore_Win32_Handler *handler_read;
   Ecore_Win32_Handler *handler_peek;

   svr = (Ecore_Con_Server *)data;

   if (!svr->pipe)
     return ECORE_CALLBACK_CANCEL;

   if (svr->dead)
     return ECORE_CALLBACK_CANCEL;

   if (svr->delete_me)
     return ECORE_CALLBACK_CANCEL;

   if ((svr->client_limit >= 0) && (!svr->reject_excess_clients) &&
       (svr->client_count >= (unsigned int)svr->client_limit))
     return ECORE_CALLBACK_CANCEL;

   cl = calloc(1, sizeof(Ecore_Con_Client));
   if (!cl)
     {
        ERR("allocation failed");
        return ECORE_CALLBACK_CANCEL;
     }

   cl->host_server = svr;
   ECORE_MAGIC_SET(cl, ECORE_MAGIC_CON_CLIENT);

   cl->host_server->event_read = CreateEvent(NULL, TRUE, FALSE, NULL);
   if (!cl->host_server->event_read)
     {
        ERR("Can not create event read");
        goto free_cl;
     }

   handler_read = ecore_main_win32_handler_add(cl->host_server->event_read,
                                               _ecore_con_local_win32_server_read_client_handler,
                                               cl);
   if (!handler_read)
     {
        ERR("Can not create handler read");
        goto close_event_read;
     }

   cl->host_server->event_peek = CreateEvent(NULL, TRUE, FALSE, NULL);
   if (!cl->host_server->event_peek)
     {
        ERR("Can not create event peek");
        goto del_handler_read;
     }

   handler_peek = ecore_main_win32_handler_add(cl->host_server->event_peek,
                                               _ecore_con_local_win32_server_peek_client_handler,
                                               cl);
   if (!handler_peek)
     {
        ERR("Can not create handler peek");
        goto close_event_peek;
     }

   cl->host_server->read_stopped = EINA_TRUE;
   cl->host_server->thread_read = (HANDLE)_beginthreadex(NULL, 0, _ecore_con_local_win32_server_read_client_thread, cl, CREATE_SUSPENDED, NULL);
   if (!cl->host_server->thread_read)
     {
        ERR("Can not launch thread");
        goto del_handler_peek;
     }

   svr->clients = eina_list_append(svr->clients, cl);
   svr->client_count++;

   if (!cl->delete_me)
     ecore_con_event_client_add(cl);

   ecore_main_win32_handler_del(wh);

   ResumeThread(cl->host_server->thread_read);
   return ECORE_CALLBACK_DONE;

 del_handler_peek:
   ecore_main_win32_handler_del(handler_peek);
 close_event_peek:
   CloseHandle(cl->host_server->event_peek);
 del_handler_read:
   ecore_main_win32_handler_del(handler_read);
 close_event_read:
   CloseHandle(cl->host_server->event_read);
 free_cl:
   free(cl);

   return ECORE_CALLBACK_CANCEL;
}

static unsigned int __stdcall
_ecore_con_local_win32_listening(void *data)
{
   Ecore_Con_Server *svr;
   BOOL res;

   svr = (Ecore_Con_Server *)data;

   while (1)
     {
        res = ConnectNamedPipe(svr->pipe, NULL);
        if (!res)
          {
             ERR("Opening the connection to the client failed");
             CloseHandle(svr->pipe);
             svr->pipe = NULL;
          }
        break;
     }

   DBG("Client connected");

   printf(" ### %s\n", __FUNCTION__);
   _endthreadex(0);
   return 0;
}

Eina_Bool
ecore_con_local_listen(Ecore_Con_Server *svr)
{
   char buf[256];
   HANDLE thread_listening;
   Ecore_Win32_Handler *handler;

   if ((svr->type & ECORE_CON_TYPE) == ECORE_CON_LOCAL_ABSTRACT)
     {
        ERR("Your system does not support abstract sockets!");
        return EINA_FALSE;
     }

   if ((svr->type & ECORE_CON_TYPE) == ECORE_CON_LOCAL_USER)
     snprintf(buf, sizeof(buf), "\\\\.\\pipe\\%s", svr->name);
   else if ((svr->type & ECORE_CON_TYPE) == ECORE_CON_LOCAL_SYSTEM)
     {
        const char *computername;

        computername = getenv("CoMPUTERNAME");
        snprintf(buf, sizeof(buf), "\\\\%s\\pipe\\%s", computername, svr->name);
     }

   svr->path = strdup(buf);
   if (!svr->path)
     {
        ERR("Allocation failed");
        return EINA_FALSE;
     }

   /*
    * synchronuous
    * block mode
    * wait mode
    */
   svr->pipe = CreateNamedPipe(svr->path,
                               PIPE_ACCESS_DUPLEX,
                               PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
                               PIPE_UNLIMITED_INSTANCES,
                               BUFSIZE,
                               BUFSIZE,
                               5000,
                               NULL);
   if (svr->pipe == INVALID_HANDLE_VALUE)
     {
        ERR("Creation of the named pipe failed");
        goto free_path;
     }

   /*
    * We use ConnectNamedPipe() to wait for a client to connect.
    * As the function is blocking, to let the main loop continuing
    * its iterations, we call ConnectNamedPipe() in a thread
    */
   thread_listening = (HANDLE)_beginthreadex(NULL, 0, _ecore_con_local_win32_listening, svr, CREATE_SUSPENDED, NULL);
   if (!thread_listening)
     {
        ERR("Creation of the listening thread failed");
        goto close_pipe;
     }

   handler = ecore_main_win32_handler_add(thread_listening,
                                          _ecore_con_local_win32_client_add,
                                          svr);
   if (!handler)
     {
        ERR("Creation of the client add handler failed");
        goto del_handler;
     }

   svr->read_stopped = EINA_TRUE;
   ResumeThread(thread_listening);

   return EINA_TRUE;

 del_handler:
   ecore_main_win32_handler_del(handler);
 close_pipe:
   CloseHandle(svr->pipe);
 free_path:
   free(svr->path);
   svr->path = NULL;

   return EINA_FALSE;
}

void
ecore_con_local_win32_server_del(Ecore_Con_Server *svr)
{
   if ((svr->type & ECORE_CON_TYPE) == ECORE_CON_LOCAL_ABSTRACT)
     return;

   if (((svr->type & ECORE_CON_TYPE) != ECORE_CON_LOCAL_USER) &&
       ((svr->type & ECORE_CON_TYPE) != ECORE_CON_LOCAL_SYSTEM))
     return;

   svr->read_stop = 1;
   while (!svr->read_stopped)
     Sleep(100);

   if (svr->event_peek)
     CloseHandle(svr->event_peek);
   svr->event_peek = NULL;
   if (svr->event_read)
     CloseHandle(svr->event_read);
   svr->event_read = NULL;
   free(svr->path);
   svr->path = NULL;
   if (svr->pipe)
     CloseHandle(svr->pipe);
   svr->pipe = NULL;
}

void
ecore_con_local_win32_client_del(Ecore_Con_Client *cl)
{
   if ((cl->host_server->type & ECORE_CON_TYPE) == ECORE_CON_LOCAL_ABSTRACT)
     return;

   if (((cl->host_server->type & ECORE_CON_TYPE) != ECORE_CON_LOCAL_USER) &&
       ((cl->host_server->type & ECORE_CON_TYPE) != ECORE_CON_LOCAL_SYSTEM))
     return;

   cl->host_server->read_stop = 1;
   while (!cl->host_server->read_stopped)
     Sleep(100);

   if (cl->host_server->event_peek)
     CloseHandle(cl->host_server->event_peek);
   cl->host_server->event_peek = NULL;
   if (cl->host_server->event_read)
     CloseHandle(cl->host_server->event_read);
   cl->host_server->event_read = NULL;
   free(cl->host_server->path);
   cl->host_server->path = NULL;
   if (cl->host_server->pipe)
     CloseHandle(cl->host_server->pipe);
   cl->host_server->pipe = NULL;
}

Eina_Bool
ecore_con_local_connect(Ecore_Con_Server *svr,
                        Eina_Bool (*cb_done)(void *data,
                                             Ecore_Fd_Handler *fd_handler))
{
   char buf[256];
   Ecore_Win32_Handler *handler_read;
   Ecore_Win32_Handler *handler_peek;

   if ((svr->type & ECORE_CON_TYPE) == ECORE_CON_LOCAL_ABSTRACT)
     {
        ERR("Your system does not support abstract sockets!");
        return EINA_FALSE;
     }

   if ((svr->type & ECORE_CON_TYPE) == ECORE_CON_LOCAL_USER)
     snprintf(buf, sizeof(buf), "\\\\.\\pipe\\%s", svr->name);
   else if ((svr->type & ECORE_CON_TYPE) == ECORE_CON_LOCAL_SYSTEM)
     {
        const char *computername;

        computername = getenv("COMPUTERNAME");
        snprintf(buf, sizeof(buf), "\\\\%s\\pipe\\%s", computername, svr->name);
     }

   while (1)
     {
        svr->pipe = CreateFile(buf,
                               GENERIC_READ | GENERIC_WRITE,
                               0,
                               NULL,
                               OPEN_EXISTING,
                               0,
                               NULL);
        if (svr->pipe != INVALID_HANDLE_VALUE)
          break;

        /* if pipe not busy, we exit */
        if (GetLastError() != ERROR_PIPE_BUSY)
          {
             ERR("Connection to a server failed");
             return EINA_FALSE;
        }

        /* pipe busy, so we wait for it */
        if (!WaitNamedPipe(buf, NMPWAIT_WAIT_FOREVER))
          {
             ERR("Can not wait for a server");
             goto close_pipe;
          }
     }

   svr->path = strdup(buf);
   if (!svr->path)
     {
        ERR("Allocation failed");
        goto close_pipe;
     }

   svr->event_read = CreateEvent(NULL, TRUE, FALSE, NULL);
   if (!svr->event_read)
     {
        ERR("Can not create event read");
        goto free_path;
     }

   handler_read = ecore_main_win32_handler_add(svr->event_read,
                                               _ecore_con_local_win32_client_read_server_handler,
                                               svr);
   if (!handler_read)
     {
        ERR("Can not create handler read");
        goto close_event_read;
     }

   svr->event_peek = CreateEvent(NULL, TRUE, FALSE, NULL);
   if (!svr->event_peek)
     {
        ERR("Can not create event peek");
        goto del_handler_read;
     }

   handler_peek = ecore_main_win32_handler_add(svr->event_peek,
                                               _ecore_con_local_win32_client_peek_server_handler,
                                               svr);
   if (!handler_peek)
     {
        ERR("Can not create handler peek");
        goto close_event_peek;
     }

   svr->thread_read = (HANDLE)_beginthreadex(NULL, 0, _ecore_con_local_win32_client_read_server_thread, svr, CREATE_SUSPENDED, NULL);
   if (!svr->thread_read)
     {
        ERR("Can not launch thread");
        goto del_handler_peek;
     }

   if (!svr->delete_me) ecore_con_event_server_add(svr);

   ResumeThread(svr->thread_read);

   return EINA_TRUE;

 del_handler_peek:
   ecore_main_win32_handler_del(handler_peek);
 close_event_peek:
   CloseHandle(svr->event_peek);
 del_handler_read:
   ecore_main_win32_handler_del(handler_read);
 close_event_read:
   CloseHandle(svr->event_read);
 free_path:
   free(svr->path);
   svr->path = NULL;
 close_pipe:
   CloseHandle(svr->pipe);

   return EINA_FALSE;
}

Eina_Bool
ecore_con_local_win32_server_flush(Ecore_Con_Server *svr)
{
   int num;
   BOOL res;
   DWORD written;

   /* This check should never be true */
   if ((svr->type & ECORE_CON_TYPE) == ECORE_CON_LOCAL_ABSTRACT)
     return EINA_TRUE;

   if (((svr->type & ECORE_CON_TYPE) != ECORE_CON_LOCAL_USER) &&
       ((svr->type & ECORE_CON_TYPE) != ECORE_CON_LOCAL_SYSTEM))
     return EINA_FALSE;

   num = eina_binbuf_length_get(svr->buf) - svr->write_buf_offset;
   if (num <= 0) return EINA_TRUE;

   res = WriteFile(svr->pipe, eina_binbuf_string_get(svr->buf) + svr->write_buf_offset, num, &written, NULL);
   if (!res)
     {
        char *msg;

        msg = evil_last_error_get();
        if (msg)
          {
             ecore_con_event_server_error(svr, msg);
             free(msg);
          }
        if (!svr->delete_me)
          ecore_con_event_server_del(svr);
        svr->dead = EINA_TRUE;
     }

   svr->write_buf_offset += written;
   if (svr->write_buf_offset >= eina_binbuf_length_get(svr->buf))
     {
        svr->write_buf_offset = 0;
	eina_binbuf_free(svr->buf);
        svr->buf = NULL;
        svr->want_write = 0;
     }
   else if (written < (DWORD)num)
     svr->want_write = 1;

   return EINA_TRUE;
}

Eina_Bool
ecore_con_local_win32_client_flush(Ecore_Con_Client *cl)
{
   Ecore_Con_Type type;
   int num;
   BOOL res;
   DWORD written;

   type = cl->host_server->type & ECORE_CON_TYPE;

   /* This check should never be true */
   if (type == ECORE_CON_LOCAL_ABSTRACT)
     return EINA_TRUE;

   if ((type != ECORE_CON_LOCAL_USER) &&
       (type != ECORE_CON_LOCAL_SYSTEM))
     return EINA_FALSE;

   num = eina_binbuf_length_get(cl->buf) - cl->buf_offset;
   if (num <= 0) return EINA_TRUE;

   res = WriteFile(cl->host_server->pipe, eina_binbuf_string_get(cl->buf) + cl->buf_offset, num, &written, NULL);
   if (!res)
     {
        char *msg;

        msg = evil_last_error_get();
        if (msg)
          {
             ecore_con_event_client_error(cl, msg);
             free(msg);
          }
        if (!cl->delete_me)
          ecore_con_event_client_del(cl);
        cl->dead = EINA_TRUE;
     }

   cl->buf_offset += written;
   if (cl->buf_offset >= eina_binbuf_length_get(cl->buf))
     {
        cl->buf_offset = 0;
        eina_binbuf_free(cl->buf);
        cl->buf = NULL;
        cl->host_server->want_write = 0;
     }
   else if (written < (DWORD)num)
     cl->host_server->want_write = 1;

   return EINA_TRUE;
}
