import { Card, CardHeader, CardBody, CardFooter } from '@paljs/ui/Card';
import Col from '@paljs/ui/Col';
import Row from '@paljs/ui/Row';
import React, { useEffect, useState } from 'react';
import SEO from '../components/SEO';
import { wsp } from '../components/websocket/websocket';
import { navigate } from 'gatsby';
import { Line, defaults } from 'react-chartjs-2'

defaults.global.tooltips.enabled = false
defaults.global.legend.position = 'bottom'


const Home = () => {
  const [allData, setAllData] = useState(null);
  useEffect(() => {
    wsp
      .open()
      .then(() => wsp.sendRequest({ MSG_TYPE: 'SHOW_ALL_SENSOR_DATA' }))
      .then((response) => setAllData(response.DATA));
  }, []);

  const cart = [
    ["Corn", "Potato", "Radish"],
    ["Tomato", "Graphes", "Mango"],
    ["Was", "soll", "das"]
  ];
    

  // return (
  //   <div>
  //     {cart.map((items, index) => {
  //       return (
  //         <ol>
  //           {items.map((subItems, sIndex) => {
  //             return <li> {subItems} </li>;
  //           })}
  //         </ol>
  //       );
  //     })}
  //   </div>
  // );

  return (
    <div>
      {allData.map((items, index) => {
        return (
          <ol>
            {items.map((subItems, sIndex) => {
              return <li> {subItems} </li>;
            })}
          </ol>
        );
      })}
    </div>
  );


  // return (
  //   <div>
  //     <SEO title="Home" />
  //     <Row>
  //       {cart
  //         ? cart.map((val, index) => {
  //             return (
  //               // val.name etc....
  //               val.map((subItems, sIndex) => {
  //                 <Col>
  //                   <Card>
  //                     <CardHeader>{subItems}</CardHeader>
  //                     <CardBody>test1</CardBody>
  //                     <CardFooter>test2</CardFooter>
  //                   </Card>
  //                 </Col>
  //                 })
  //             );
  //           })
  //         : 'Noch keine Daten angelegt'}
  //     </Row>
  //   </div>
  // );
};
export default Home;
