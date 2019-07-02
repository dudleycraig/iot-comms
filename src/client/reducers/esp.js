/** reducers/espTypes */

import espTypes from 'types/esp'

const initialState = {
  status: 'disconnected',
  readyState: null,
  message: null,
  socket: null,
  error: null,
}

const esp = (state = initialState, action = {}) => {
  switch (action.type) {
    case espTypes.CONNECT:
      return {
        ...state,
        status: 'connecting',
        socket: action.socket,
      }

    case espTypes.DISCONNECT:
      return {
        ...state,
        status: 'disconnecting',
        socket: null,
      }

    case espTypes.HANDLE_REQUEST:
      return {
        ...state,
        status: 'handling-request',
        message: action.message,
      }

    case espTypes.HANDLE_RESPONSE:
      return {
        ...state,
        status: 'handle-respTypesonse',
        message: action.message,
      }

    case espTypes.SEND_REQUEST:
      return {
        ...state,
        status: 'send-request',
        message: action.message,
      }

    case espTypes.SEND_RESPONSE:
      return {
        ...state,
        status: 'send-respTypesonse',
        message: action.message,
      }

    case espTypes.HANDLE_ERROR:
      return {
        ...state,
        status: 'error',
        error: action.error,
      }

    case espTypes.SET_STATUS:
      return {
        ...state,
        status: action.status,
      }

    default:
      return state
  }
}

export default esp
