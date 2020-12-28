import { request } from 'http';
import WebSocketAsPromised from 'websocket-as-promised';
const WEBSOCKET_URL = 'ws://localhost:5000/ws';
export const wsp = new WebSocketAsPromised(WEBSOCKET_URL, {
  packMessage: (data) => JSON.stringify(data),
  unpackMessage: (data) => JSON.parse(data),
  attachRequestId: (data, requestId) => Object.assign({ ID: requestId }, data),
  extractRequestId: (data) => data && data.ID,
});
