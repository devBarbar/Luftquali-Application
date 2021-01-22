import { Card, CardHeader, CardBody, CardFooter } from '@paljs/ui/Card';
import Col from '@paljs/ui/Col';
import Row from '@paljs/ui/Row';
import React, { useEffect, useState } from 'react';
import SEO from '../components/SEO';
import { wsp } from '../components/websocket/websocket';
import { navigate } from 'gatsby';

const Home = () => {
  const [allRooms, setAllRooms] = useState(null);
  useEffect(() => {
    wsp
      .open()
      .then(() => wsp.sendRequest({ MSG_TYPE: 'SHOW_ALL_ROOMS' })) // actually sends {foo: 'bar', id: 'xxx'}, because `attachRequestId` defined above
      .then((response) => setAllRooms(response.DATA)); // waits server message with corresponding requestId: {id: 'xxx', ...}
  }, []);

  const loadRoomDataFromDatabase = function (name) {
    wsp
      .open()
      .then(() => wsp.sendRequest({ MSG_TYPE: 'GET_ROOM_FROM_DATABASE', ROOM_NAME: name }))
      .then((response) => {
        response.DATA && window.localStorage.setItem('react-planner_v0', response.DATA);
        navigate('/floorplanner');
      });
  };
  return (
    <div>
      <SEO title="Home" />
      <Row>
        {allRooms
          ? allRooms.map((val, index) => {
              return (
                // val.name etc....
                <Col onClick={(e) => loadRoomDataFromDatabase(val)} breakPoint={{ xs: 12, md: 6 }}>
                  <Card>
                    <CardHeader>{val}</CardHeader>
                    <CardBody>Funkmodul Outputs: __VAR__</CardBody>
                    <CardFooter>Letzter Alarm: __VAR__</CardFooter>
                  </Card>
                </Col>
              );
            })
          : 'Noch keinen Raum angelegt'}
      </Row>
    </div>
  );
};
export default Home;
