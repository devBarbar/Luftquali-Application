import React, { useEffect, useState } from 'react';
import PropTypes from 'prop-types';
import Select from '@paljs/ui/Select';
import { Card, CardBody } from '@paljs/ui/Card';
import styled from 'styled-components';
import PropertyStyle from './shared-property-style';
import { wsp } from '../../websocket/websocket';
import { Link } from 'gatsby';

export const SelectStyled = styled(Select)`
    display: block;
    width: 100%;
    padding: 0px 2px;
    font-size: 13px;
    line-height: 1.25;
    color: rgb(85, 89, 92);
    background-color: rgb(255, 255, 255);
    background-image: none;
    border: 1px solid rgba(0, 0, 0, 0.15);
    outline: none;
    height: 30px;

  
`;

export default function SensorProperty({value, onUpdate, configs, sourceElement, internalState, state}) {
    const [availableSensors, setAvailableSensors] = useState(null);
    const [activeSensor, setActiveSensor] = useState(null);
    useEffect(() => {
        wsp
          .open()
          .then(() => wsp.sendRequest({ MSG_TYPE: 'GET_AVAILABLE_SENSORS' })) // actually sends {foo: 'bar', id: 'xxx'}, because `attachRequestId` defined above
          .then((response) => setAvailableSensors(response.DATA)); // waits server message with corresponding requestId: {id: 'xxx', ...}
      }, []);

  let update = (val) => {

    if (configs.hook) {
      return configs.hook(val, sourceElement, internalState, state).then(_val => {
        return onUpdate(_val);
      });
    }

    return onUpdate(val);
  };

  const statusOption = [
    { label: 'Clean' },
    { value: 'Info', label: 'Info' },
    { value: 'Success', label: 'Success' },
    { value: 'Danger', label: 'Danger' },
    { value: 'Primary', label: 'Primary' },
    { value: 'Warning', label: 'Warning' },
    { value: 'Default', label: 'Default' },
  ];
  const handleSensorChange = function(value) {
    setActiveSensor(value);
    typeof window !== "undefined" && window.localStorage.setItem('active_statistic_sensor', value)
    update(value)
  }

  return (
      availableSensors && (
    <table className="PropertyColor" style={PropertyStyle.tableStyle}>
    <tbody>
    <tr>
      <td style={PropertyStyle.firstTdStyle}>
        <label>{configs.label}</label>
      </td>
      <td>
          
      <select onChange={event => handleSensorChange(event.target.value) } value={value} options={statusOption} placeholder={configs.label} >
          {availableSensors.map(val => <option value={val}>{val}</option>)}
          </select>
      </td>
      <td>
        <Link to="/statistics">Show Statistics</Link>
      </td>
    </tr>
    </tbody>
  </table>
  )
  );
}

SensorProperty.propTypes = {
  value: PropTypes.any.isRequired,
  onUpdate: PropTypes.func.isRequired,
  configs: PropTypes.object.isRequired,
  sourceElement: PropTypes.object,
  internalState: PropTypes.object,
  state: PropTypes.object.isRequired
};