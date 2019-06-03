/** containers/home **/

import React, { Component } from 'react'
import { connect } from 'react-redux'
import styled, { css } from 'styled-components'
import Socket from 'components/Socket'
import {
  initialize as wsInitialize,
  connect as wsConnect,
  disconnect as wsDisconnect,
  sendMessage as wsSendMessage,
  handleMessage as wsHandleMessage,
  handleError as wsHandleError,
  setStatus as wsSetStatus,
} from 'actions/socket'

class Home extends Component {

  componentWillMount() {
    this.props.initialize('ws://10.0.0.144:81')
  }

  sendMessage(message) {
    const {
      socket: {
        message: wsMessage,
        socket: wsSocket,
      },
    } = this.props
    wsSocket.send(message)
  }

  render() {
    const {
      socket: {
        status: wsStatus,
        readyState: wsReadyState,
        message: wsMessage,
        error: wsError,
      },
    } = this.props

    return (
      <main id="home">
        <h1>Socket</h1>
        <Socket status={wsStatus} />
        <br />
        <button onClick={() => this.sendMessage('ledon')}>On</button>
        &nbsp;
        <button onClick={() => this.sendMessage('ledoff')}>Off</button>
      </main>
    )
  }
}

export default connect(
  state => ({
    socket: state.socket,
  }),
  {
    initialize: wsInitialize,
    connect: wsConnect,
    disconnect: wsDisconnect,
    sendMessage: wsSendMessage,
    handleMessage: wsHandleMessage,
    handleError: wsHandleError,
    setStatus: wsSetStatus,
  }
)(Home)
