import { Catalog } from 'react-planner';

let catalog = new Catalog();

import * as Areas from './areas/**/planner-element.jsx';
import * as Lines from './lines/**/planner-element.jsx';
import * as Holes from './holes/**/planner-element.jsx';
import * as Items from './items/**/planner-element.jsx';

for (let x in Areas) catalog.registerElement(Areas[x]);
for (let x in Lines) catalog.registerElement(Lines[x]);
catalog.registerElement(Holes.door);
catalog.registerElement(Holes.doorDouble);
catalog.registerElement(Holes.window);
catalog.registerElement(Items.airConditioner);
catalog.registerElement(Items.radiatorModernStyle);
catalog.registerElement(Items.smokeDetector);
catalog.registerElement(Items.cube);
export default catalog;
