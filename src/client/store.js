/** store * */

import thunkMiddleware from 'redux-thunk';
import { createStore, combineReducers, applyMiddleware, compose } from 'redux';
import app from 'reducers/app';
import socket from 'reducers/socket';

export const store = createStore(
  combineReducers({ 
    app,
    socket
  }), applyMiddleware(thunkMiddleware),
);

export default store;
