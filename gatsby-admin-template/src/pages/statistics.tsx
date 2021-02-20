import { Card, CardHeader, CardBody, CardFooter } from '@paljs/ui/Card';
import Col from '@paljs/ui/Col';
import Row from '@paljs/ui/Row';
import React, { useEffect, useState } from 'react';
import SEO from '../components/SEO';
import { wsp } from '../components/websocket/websocket';
import { navigate } from 'gatsby';
import { ResponsiveLine } from '@nivo/line'

const Statistics = () => {
  const [data2, setData] = useState(null);
  const [statistics,setStatistics] = useState({"MIN":0,"MAX":0,"AVERAGE":0})
  useEffect(() => {
    wsp
      .open()
      .then(() => wsp.sendRequest({ MSG_TYPE: 'GET_STATISTICS_FOR_SPECIFIC_SENSOR',SENSOR:window.localStorage.getItem('active_statistic_sensor') })) // actually sends {foo: 'bar', id: 'xxx'}, because `attachRequestId` defined above
      .then((response) => {
        setData(response.DATA[0])
        setStatistics(response.DATA[1])
      } ); // waits server message with corresponding requestId: {id: 'xxx', ...}
      const interval = setInterval(() => {
        wsp
          .open()
          .then(() => wsp.sendRequest({ MSG_TYPE: 'GET_STATISTICS_FOR_SPECIFIC_SENSOR',SENSOR:window.localStorage.getItem('active_statistic_sensor') })) // actually sends {foo: 'bar', id: 'xxx'}, because `attachRequestId` defined above
          .then((response) => {
            setData(response.DATA[0])
            setStatistics(response.DATA[1])
          } ); // waits server message with corresponding requestId: {id: 'xxx', ...} 
      },10000)
  }, []);

console.log(data2)
  return (
      data2 && 
    <div style={{height:"100vh"}} className="testst">
      <ResponsiveLine
        data={data2}
        margin={{ top: 50, right: 110, bottom: 50, left: 60 }}
        xScale={{ type: 'point' }}
        yScale={{ type: 'linear', min: 'auto', max: 'auto', stacked: true, reverse: false }}
        yFormat=" >-.2f"
        axisTop={null}
        axisRight={null}
        axisBottom={{
            orient: 'bottom',
            tickSize: 5,
            tickPadding: 5,
            tickRotation: 0,
            legend: 'Zeit',
            legendOffset: 36,
            legendPosition: 'middle'
        }}
        axisLeft={{
            orient: 'left',
            tickSize: 5,
            tickPadding: 5,
            tickRotation: 0,
            legend: 'Wert',
            legendOffset: -40,
            legendPosition: 'middle'
        }}
        pointSize={10}
        pointColor={{ theme: 'background' }}
        pointBorderWidth={2}
        pointBorderColor={{ from: 'serieColor' }}
        pointLabelYOffset={-12}
        useMesh={true}
        legends={[
            {
                anchor: 'bottom-right',
                direction: 'column',
                justify: false,
                translateX: 100,
                translateY: 0,
                itemsSpacing: 0,
                itemDirection: 'left-to-right',
                itemWidth: 80,
                itemHeight: 20,
                itemOpacity: 0.75,
                symbolSize: 12,
                symbolShape: 'circle',
                symbolBorderColor: 'rgba(0, 0, 0, .5)',
                effects: [
                    {
                        on: 'hover',
                        style: {
                            itemBackground: 'rgba(0, 0, 0, .03)',
                            itemOpacity: 1
                        }
                    }
                ]
            }
        ]}
    />
    <Row>
        {statistics && 
        <Col  breakPoint={{ xs: 12, md: 6 }}>
        <Card>
          <CardHeader>Statistiken</CardHeader>
          <CardBody>
              MIN:{statistics["MIN"]}
              MAX:{statistics["MAX"]}
              DURCHSCHNITT:{statistics["AVERAGE"]}
            </CardBody>
          <CardFooter>Letzter Alarm: __VAR__</CardFooter>
        </Card>
      </Col>
        }
      </Row>
    </div>
  );
};
export default Statistics;
