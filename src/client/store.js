/** store */

import thunkMiddleware from 'redux-thunk'
import { createStore, combineReducers, applyMiddleware, compose } from 'redux'
import app from 'reducers/app'
import esp from 'reducers/esp'

const store = createStore(
  combineReducers({
    app,
    esp,
  }),
  applyMiddleware(thunkMiddleware),
)

export default store
