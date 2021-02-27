import { Card, CardHeader, CardBody, CardFooter } from '@paljs/ui/Card';
import Col from '@paljs/ui/Col';
import Row from '@paljs/ui/Row';
import React, { useEffect, useState } from 'react';
import SEO from '../../components/SEO';
import { wsp } from '../../components/websocket/websocket';

const Home = () => {
  const [allRooms, setAllRooms] = useState(null);
  useEffect(() => {
    wsp
      .open()
      .then(() => wsp.sendRequest({ MSG_TYPE: 'GET_ALL_SETTINGS' })) // actually sends {foo: 'bar', id: 'xxx'}, because `attachRequestId` defined above
      .then((response) => setAllRooms(response.DATA)); // waits server message with corresponding requestId: {id: 'xxx', ...}
  }, []);

  const loadRoomDataFromDatabase = function () {
    wsp.open().then(() => wsp.sendRequest({ MSG_TYPE: 'GET_ROOM_FROM_DATABASE', ROOM_NAME: '' }));
  };
  return (
    <div>
      <SEO title="Home" />
      <Row>
        {allRooms
          ? allRooms.map((val, index) => {
              return (
                <Col breakPoint={{ xs: 12, md: 6 }}>
                  <Card>
                    <CardHeader>{val}</CardHeader>
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
