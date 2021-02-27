import { Catalog } from 'react-planner';

import * as Areas from './areas/**/planner-element.jsx';
import * as Lines from './lines/**/planner-element.jsx';
import * as Holes from './holes/**/planner-element.jsx';
import * as Items from './items/**/planner-element.jsx';
import SensorProperty from './sensorProperty.js';
import { isBrowser } from '../../../services/auth.js';
let catalog = isBrowser() && new Catalog();
if (catalog) {
  for (let x in Areas) catalog.registerElement(Areas[x]);
  for (let x in Lines) catalog.registerElement(Lines[x]);
  catalog.registerPropertyType('Sensor', SensorProperty, SensorProperty);
  catalog.registerElement(Holes.window);
  //catalog.registerElement(Items.airConditioner);
  //catalog.registerElement(Items.radiatorModernStyle);
  //catalog.registerElement(Items.smokeDetector);
  catalog.registerElement(Items.cube);

  console.log(catalog);
}
export default catalog;
