/** containers/App.js * */

import React, { Component, PropTypes } from 'react';
import { connect } from 'react-redux';
import { HashRouter as Router, Switch, Route } from 'react-router-dom';
import Home from 'containers/Home';

class App extends Component {
  render() { 
    return (
      <Router>
        <div id="app">
          {
            <Switch>
              <Route exact path="/" component={Home} />
            </Switch>
          }
        </div>
      </Router>
    )
  }
}

export default connect(
  state => ({
    app: state.app,
  }),
  { },
)(App)
