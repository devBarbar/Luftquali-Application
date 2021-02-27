import React from 'react';
import ContainerDimensions from 'react-container-dimensions';
import Immutable, { Map } from 'immutable';
import immutableDevtools from 'immutable-devtools';
import { createStore } from 'redux';
import { Provider } from 'react-redux';

import MyCatalog from './catalog/mycatalog';

import ToolbarSaveToDatabaseButton from './ui/toolbar-save-to-database';

import {
  Models as PlannerModels,
  reducer as PlannerReducer,
  ReactPlanner,
  Plugins as PlannerPlugins,
} from 'react-planner'; //react-planner
import { isBrowser } from '../../services/auth';

//define state
let AppState = Map({
  'react-planner': isBrowser() ? new PlannerModels.State() : null,
});

//define reducer
let reducer = (state, action) => state;
if (isBrowser()) {
  reducer = (state, action) => {
    state = state || AppState;
    state = state.update('react-planner', (plannerState) => PlannerReducer(plannerState, action));
    return state;
  };
}
let blackList = isProduction === true ? [] : ['UPDATE_MOUSE_COORDS', 'UPDATE_ZOOM_SCALE', 'UPDATE_2D_CAMERA'];

if (!isProduction) {
  console.info('Environment is in development and these actions will be blacklisted', blackList);
  console.info('Enable Chrome custom formatter for Immutable pretty print');
  immutableDevtools(Immutable);
}

//init store
let store = createStore(
  reducer,
  null,
  !isProduction && window.devToolsExtension
    ? window.devToolsExtension({
        features: {
          pause: true, // start/pause recording of dispatched actions
          lock: true, // lock/unlock dispatching actions and side effects
          persist: true, // persist states on page reloading
          export: false, // export history of actions in a file
          import: 'custom', // import history of actions from a file
          jump: true, // jump back and forth (time travelling)
          skip: true, // skip (cancel) actions
          reorder: true, // drag and drop actions in the history list
          dispatch: true, // dispatch custom actions or action creators
          test: true, // generate tests for the selected actions
        },
        actionsBlacklist: blackList,
        maxAge: 999999,
      })
    : (f) => f,
);
let plugins = [];
if (isBrowser()) {
  plugins = [PlannerPlugins.Keyboard(), PlannerPlugins.Autosave('react-planner_v0'), PlannerPlugins.ConsoleDebugger()];
}
let toolbarButtons = [ToolbarSaveToDatabaseButton];

function Floorplanner() {
  return (
    <Provider store={store}>
      <ContainerDimensions>
        {({ width, height }) => (
          <ReactPlanner
            catalog={MyCatalog}
            width={width}
            height={800}
            plugins={plugins}
            toolbarButtons={toolbarButtons}
            stateExtractor={(state) => state.get('react-planner')}
          />
        )}
      </ContainerDimensions>
    </Provider>
  );
}

export default Floorplanner;
