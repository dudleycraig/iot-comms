/** actions/socket **/

import { types } from 'types/socket'

export const connect = socket => ({
  type: types.CONNECT,
  socket,
})

export const disconnect = () => ({
  type: types.DISCONNECT,
})

export const sendMessage = () => ({
  type: types.SEND_MESSAGE,
})

export const handleMessage = () => ({
  type: types.SEND_MESSAGE,
})

export const handleError = (error = null) => ({
  type: types.HANDLE_ERROR,
  error,
})

export const setStatus = status => ({
  type: types.SET_STATUS,
  status,
})

export const initialize = (url) => {
  return (dispatch) => {
    const socket = new WebSocket(url)

    dispatch(connect(socket))

    socket.onopen = (event) => {
      console.log('socket.onopen', event)
      dispatch(setStatus('connected'))
    }

    socket.onclose = (event) => {
      console.log('socket.onclose', event)
      dispatch(setStatus('disconnected'))
    }

    socket.onmessage = (event) => {
      console.log('socket.onmessage', event)
      dispatch(handleMessage(event.data))
    }

    socket.onerror = (event) => {
      console.log('socket.onerror', event)
      dispatch(handleError(event.error))
    }
  }
}
