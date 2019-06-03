/** components/Socket.js **/

import React from 'react';
import styled, { css } from 'styled-components'

const Socket = props => {
  const {
    status,
    message,
  } = props

  const statusColor = (status) => {
    const colors = {
      RED: '#ff0000',
      ORANGE: '#ffac00',
      GREEN: '#006400',
      INFO: '#778899',
    }

    switch (status) {
      case ('error'): return colors.RED
      case ('connecting'): return colors.ORANGE
      case ('connected'): return colors.GREEN
      case ('disconnecting'): return colors.ORANGE
      case ('disconnected'): return colors.GREEN
      case ('processing-message'): return colors.ORANGE
      case ('processed-message'): return colors.GREEN
      case ('response'): return colors.GREEN
      case ('responding'): return colors.ORANGE
      default: return null
    }
  }

  const Container = styled.dl`
    display: inline-block;
    width: 100%;
    clear: none;
    border: 1px solid #000000;
    padding: 3px;
    background-color: #cfcfcf;
  `

  const Term = styled.dt`
    width: 30%;
    float: left;
    clear: left;
    text-align: right;
    padding-right: 10px;
    color: #000000;
  `

  const Description = styled.dd`
    width: 60%;
    float: left;
    clear: right;
    text-align: left;
  `

  const WSStatusDescription = styled(Description)`
    color: ${statusColor(status)};
  `

  return (
    <Container>
      <Term>status:</Term>
      <Description>{status}</Description>
    </Container>
  )
}

export default Socket
