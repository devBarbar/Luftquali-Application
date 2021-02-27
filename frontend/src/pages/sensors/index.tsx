import { Card, CardHeader, CardBody, CardFooter } from '@paljs/ui/Card';
import Col from '@paljs/ui/Col';
import Row from '@paljs/ui/Row';
import React, { useEffect, useState } from 'react';
import SEO from '../../components/SEO';
import { wsp } from '../../components/websocket/websocket';
import { navigate } from 'gatsby';

const Sensors = () => {
  const [allSensors, setAllSensors] = useState(null);
  useEffect(() => {
    wsp
      .open()
      .then(() => wsp.sendRequest({ MSG_TYPE: 'GET_AVAILABLE_SENSORS' }))
      .then((response) => setAllSensors(response.DATA));
  }, []);

  const getStatisticsForSpecificSensor = function (name) {
    typeof window !== 'undefined' && window.localStorage.setItem('active_statistic_sensor', name);
    navigate('/statistics');
  };
  return (
    <div>
      <SEO title="Sensors" />
      <Row>
        {allSensors
          ? allSensors.map((val, index) => {
              return (
                <Col onClick={(e) => getStatisticsForSpecificSensor(val)} breakPoint={{ xs: 12, md: 6 }}>
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
export default Sensors;
