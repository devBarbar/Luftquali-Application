import { wsp } from '../components/websocket/websocket';

export const isBrowser = () => typeof window !== 'undefined';

export const getUser = () =>
  isBrowser() && window.localStorage.getItem('airUser') ? JSON.parse(window.localStorage.getItem('airUser')) : {};

export const setUser = (user) => isBrowser() && window.localStorage.setItem('airUser', JSON.stringify(user));

export const handleLogin = (username, password) => {
  const loginHandler = wsp
    .open()
    .then(() => wsp.sendRequest({ MSG_TYPE: 'LOGIN', EMAIL: username, PASSWORD: password })); // actually sends {foo: 'bar', id: 'xxx'}, because `attachRequestId` defined above

  return loginHandler;
};

export const isLoggedIn = () => {
  const user = getUser();

  return !!user.username;
};

export const logout = (callback) => {
  setUser({});
  callback();
};
