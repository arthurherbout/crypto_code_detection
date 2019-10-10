//
// Created on 2/21/18.
//

#include <cstdlib>
#include "UvUtil.h"

uv_signal_s *UvUtil::WatchSignal(uv_loop_t *loop, int sig, uv_signal_cb cb, void *data) {
    uv_signal_t *uv_signal = static_cast<uv_signal_t *>(malloc(sizeof(uv_signal_t)));
    if (uv_signal_init(loop, uv_signal)) {
        uv_close(reinterpret_cast<uv_handle_t *>(uv_signal), close_cb);
        return nullptr;
    }
    uv_signal->data = data;
    if (uv_signal_start(uv_signal, cb, sig)) {
        uv_close(reinterpret_cast<uv_handle_t *>(uv_signal), close_cb);
        return nullptr;
    }
    return uv_signal;
}

void UvUtil::close_cb(uv_handle_t *handle) {
    free(handle);
}

void UvUtil::stop_and_close_loop_fully(uv_loop_t *loop) {
    uv_stop(loop);
    uv_walk(loop, close_walk_cb, nullptr);
    uv_loop_close(loop);
}

void UvUtil::close_walk_cb(uv_handle_t *handle, void *arg) {
    if (!uv_is_closing(handle)) {
        uv_close(handle, nullptr);  // should be deleted by itself
    }
}
