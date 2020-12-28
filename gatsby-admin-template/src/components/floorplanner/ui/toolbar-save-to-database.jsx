import React from 'react';
import PropTypes from 'prop-types';
import { BiData } from 'react-icons/bi';
import { ReactPlannerComponents, ReactPlannerConstants } from 'react-planner';
import { wsp } from '../../websocket/websocket';
const {
  MODE_IDLE,
  MODE_2D_ZOOM_IN,
  MODE_2D_ZOOM_OUT,
  MODE_2D_PAN,
  MODE_WAITING_DRAWING_LINE,
  MODE_DRAGGING_LINE,
  MODE_DRAGGING_VERTEX,
  MODE_DRAGGING_ITEM,
  MODE_DRAWING_LINE,
  MODE_DRAWING_HOLE,
  MODE_DRAWING_ITEM,
  MODE_DRAGGING_HOLE,
  MODE_ROTATING_ITEM,
  MODE_3D_FIRST_PERSON,
  MODE_3D_VIEW,
} = ReactPlannerConstants;

const { ToolbarButton } = ReactPlannerComponents.ToolbarComponents;

export default function ToolbarSaveToDatabaseButton({ mode }, { translator }) {
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

  if ([MODE_3D_FIRST_PERSON, MODE_3D_VIEW].includes(mode)) {
    return (
      <ToolbarButton active={false} tooltip={translator.t('Speichern')} onClick={saveScreenshotToFile}>
        <BiData />
      </ToolbarButton>
    );
  }

  if (
    [
      MODE_IDLE,
      MODE_2D_ZOOM_IN,
      MODE_2D_ZOOM_OUT,
      MODE_2D_PAN,
      MODE_WAITING_DRAWING_LINE,
      MODE_DRAGGING_LINE,
      MODE_DRAGGING_VERTEX,
      MODE_DRAGGING_ITEM,
      MODE_DRAWING_LINE,
      MODE_DRAWING_HOLE,
      MODE_DRAWING_ITEM,
      MODE_DRAGGING_HOLE,
      MODE_ROTATING_ITEM,
    ].includes(mode)
  ) {
    return (
      <ToolbarButton active={false} tooltip={translator.t('Speichern')} onClick={saveToDatabase}>
        <BiData />
      </ToolbarButton>
    );
  }

  return null;
}

ToolbarSaveToDatabaseButton.propTypes = {
  mode: PropTypes.string.isRequired,
};

ToolbarSaveToDatabaseButton.contextTypes = {
  translator: PropTypes.object.isRequired,
};
