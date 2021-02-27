import { Card, CardHeader, CardBody, CardFooter } from '@paljs/ui/Card';
import Col from '@paljs/ui/Col';
import Row from '@paljs/ui/Row';
import React, { useEffect, useState } from 'react';
import SEO from '../components/SEO';
import { wsp } from '../components/websocket/websocket';
import { navigate } from 'gatsby';
import { ResponsiveLine } from '@nivo/line';
import DateFnsUtils from '@date-io/date-fns';
import { styled } from '@material-ui/core/styles';
import { createMuiTheme } from '@material-ui/core';
import lime from '@material-ui/core/colors/lime';
import DatePicker from 'react-datepicker';
import 'react-datepicker/dist/react-datepicker.css';

import { MuiPickersUtilsProvider, KeyboardTimePicker, KeyboardDatePicker } from '@material-ui/pickers';
import { Button } from '@paljs/ui/Button';
import { Container } from '@nivo/core';

const Statistics = () => {
  const [data2, setData] = useState(null);
  const [statistics, setStatistics] = useState();
  const [fromDate, setFromDate] = useState(new Date());
  const [toDate, setToDate] = useState();

  const fetchData = () => {
    wsp
      .open()
      .then(() =>
        wsp.sendRequest({
          MSG_TYPE: 'GET_STATISTICS_FOR_SPECIFIC_SENSOR',
          SENSOR: window.localStorage.getItem('active_statistic_sensor'),
          FROM: fromDate,
          TO: toDate,
        }),
      ) // actually sends {foo: 'bar', id: 'xxx'}, because `attachRequestId` defined above
      .then((response) => {
        console.log(response.DATA[1]);
        setData(response.DATA[0]);
        setStatistics(response.DATA[1]);
      }); // waits server message with corresponding requestId: {id: 'xxx', ...}
    let interval = null;
    if (!toDate) {
      interval = setInterval(() => {
        wsp
          .open()
          .then(() =>
            wsp.sendRequest({
              MSG_TYPE: 'GET_STATISTICS_FOR_SPECIFIC_SENSOR',
              SENSOR: window.localStorage.getItem('active_statistic_sensor'),
              FROM: fromDate,
            }),
          ) // actually sends {foo: 'bar', id: 'xxx'}, because `attachRequestId` defined above
          .then((response) => {
            setData(response.DATA[0]);
            setStatistics(response.DATA[1]);
          }); // waits server message with corresponding requestId: {id: 'xxx', ...}
      }, 10000);
    }
    return interval;
  };

  useEffect(() => {
    console.log(statistics);
  }, [statistics]);
  const handleFromDateChange = (date) => {
    setFromDate(date);
  };
  return data2 ? (
    <>
      <div style={{ height: '100vh' }} className="testst">
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
            legendPosition: 'middle',
          }}
          axisLeft={{
            orient: 'left',
            tickSize: 5,
            tickPadding: 5,
            tickRotation: 0,
            legend: 'Wert',
            legendOffset: -40,
            legendPosition: 'middle',
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
                    itemOpacity: 1,
                  },
                },
              ],
            },
          ]}
        />
      </div>
      <Row>
        {statistics &&
          statistics.map((val) => (
            <Col breakPoint={{ xs: 12, md: 6 }}>
              <Card>
                <CardHeader>Statistiken ({val['id']})</CardHeader>
                <CardBody>
                  <p>MIN:{val['DATA']['MIN']}</p>
                  <p>MAX:{val['DATA']['MAX']}</p>
                  <p>DURCHSCHNITT:{val['DATA']['AVG']}</p>
                </CardBody>
                <CardFooter>Letzter Alarm: Kein Alarm</CardFooter>
              </Card>
            </Col>
          ))}
      </Row>
    </>
  ) : (
    <MuiPickersUtilsProvider utils={DateFnsUtils}>
      <Container>
        <Row>
          <Col xs={6}>
            <span>Von: </span>
            <DatePicker selected={fromDate} onChange={(date) => setFromDate(date)} />
          </Col>
          <Col xs={6}>
            <span>Bis: </span>
            <DatePicker selected={toDate} onChange={(date) => setToDate(date)} />
          </Col>
        </Row>
        <Row style={{ marginTop: '15px' }}>
          <Col>
            <Button onClick={fetchData}>Statistiken anzeigen</Button>
          </Col>
        </Row>
      </Container>
    </MuiPickersUtilsProvider>
  );
};
export default Statistics;
