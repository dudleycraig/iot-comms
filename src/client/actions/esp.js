/** actions/esp */

import espTypes from 'types/esp'

export const connect = (socket) => ({
  type: espTypes.CONNECT,
  socket,
})

export const disconnect = () => ({
  type: espTypes.DISCONNECT,
})

export const sendMessage = () => ({
  type: espTypes.SEND_MESSAGE,
})

export const handleMessage = () => ({
  type: espTypes.SEND_MESSAGE,
})

export const handleError = (error = null) => ({
  type: espTypes.HANDLE_ERROR,
  error,
})

export const setStatus = (status) => ({
  type: espTypes.SET_STATUS,
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
