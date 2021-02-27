import React from 'react';
import PropTypes from 'prop-types';
import { BiData } from 'react-icons/bi';
import { ReactPlannerComponents, ReactPlannerConstants } from 'react-planner';
import { wsp } from '../../websocket/websocket';
import { isBrowser } from '../../../services/auth';

export default function ToolbarSaveToDatabaseButton({ mode }, { translator }) {
  if (!isBrowser()) {
    return;
  }
  const { ToolbarButton } = ReactPlannerComponents.ToolbarComponents;
  let saveToDatabase = (event) => {
    event.preventDefault();

    const RoomName = window.prompt('Wie ist der Name des Raumes');
    const plannerStorageData = localStorage.getItem('react-planner_v0');
    wsp
      .open()
      .then(() =>
        wsp.sendRequest({ MSG_TYPE: 'SAVE_ROOM_TO_DATABASE', ROOM_NAME: RoomName, ROOM_DATA: plannerStorageData }),
      ) // actually sends {foo: 'bar', id: 'xxx'}, because `attachRequestId` defined above
      .then((response) => console.log(response)); // waits server message with corresponding requestId: {id: 'xxx', ...}
  };
  return (
    <ToolbarButton active={false} tooltip={translator.t('Speichern')} onClick={saveToDatabase}>
      <BiData />
    </ToolbarButton>
  );
}

ToolbarSaveToDatabaseButton.propTypes = {
  mode: PropTypes.string.isRequired,
};

ToolbarSaveToDatabaseButton.contextTypes = {
  translator: PropTypes.object.isRequired,
};
