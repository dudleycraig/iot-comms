/** reducers/socket **/

import { types } from 'types/socket'

const initialState = {
  status: 'disconnected',
  readyState: null,
  message: null,
  socket: null,
  error: null,
}

export default (state = initialState, action = {}) => {

  switch (action.type) {

  case types.CONNECT:
    return {
      ...state,
      status: 'connecting',
      socket: action.socket,
    }

  case types.DISCONNECT:
    return {
      ...state,
      status: 'disconnecting',
      socket: null,
    }

  case types.HANDLE_REQUEST:
    return {
      ...state,
      status: 'handling-request',
      message: action.message,
    }

  case types.HANDLE_RESPONSE:
    return {
      ...state,
      status: 'handle-response',
      message: action.message,
    }

  case types.SEND_REQUEST:
    return {
      ...state,
      status: 'send-request',
      message: action.message,
    }

  case types.SEND_RESPONSE:
    return {
      ...state,
      status: 'send-response',
      message: action.message,
    }

  case types.HANDLE_ERROR:
    return {
      ...state,
      status: 'error',
      error: action.error,
    }

  case types.SET_STATUS:
    return {
      ...state,
      status: action.status,
    }

  default:
    return state
  }
}
