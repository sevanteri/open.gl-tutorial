#ifndef INOTIFYWATCH_H
#define INOTIFYWATCH_H

#include <unistd.h>
#include <sys/inotify.h>
#include <sys/epoll.h>
#include <SDL2/SDL.h>

#include <err.h>

#define MAXEVENTS 10
#define EVENTSIZE sizeof(struct inotify_event)
#define EVBUFLEN MAXEVENTS * (EVENTSIZE)

typedef void* (*voidfunc) (void* arg);
typedef struct watcher_t {
    int inotify_fd;
    int epoll_fd;
    int watch_fd;
    char *filename;
    voidfunc func;
    void *arg;
} watcher_t;

static int
runWatcher(void* arg)
{
    struct watcher_t *watcher = (struct watcher_t*)arg;

    struct epoll_event eev = {0};
    struct epoll_event evs[MAXEVENTS];

    eev.events = EPOLLIN;
    eev.data.fd = watcher->inotify_fd;
    if (epoll_ctl(
            watcher->epoll_fd,
            EPOLL_CTL_ADD,
            watcher->inotify_fd,
            &eev
        ) == -1)
        err(1, "epoll_ctl fail");

    char buf[EVBUFLEN];
    while (1) {
        epoll_wait(watcher->epoll_fd, evs, MAXEVENTS, -1);

        read(watcher->inotify_fd, buf, EVBUFLEN);
        /*fprintf(stderr, "call\n");*/
        watcher->func(watcher->arg);
    }

    return 0;
}

static int
watchFile(watcher_t *watcher, char *filename, voidfunc func, void *arg)
{
    watcher->filename = filename;
    watcher->func = func;
    watcher->arg = arg;

    if((watcher->inotify_fd = inotify_init1(IN_NONBLOCK)) == -1)
        err(1, "inotify_init fail");

    watcher->watch_fd = inotify_add_watch(
        watcher->inotify_fd,
        watcher->filename,
        IN_CLOSE_WRITE
    );
    if (watcher->watch_fd == -1)
        err(1, "inotify_add_watch fail");

    if ((watcher->epoll_fd = epoll_create1(0)) == -1)
        err(1, "epoll_create1 fail");

    SDL_Thread *th = SDL_CreateThread(runWatcher, "inotify watcher", (void*)watcher);
    SDL_DetachThread(th);

    return 0;
}

#endif
