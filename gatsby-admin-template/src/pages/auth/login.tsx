import { Button } from '@paljs/ui/Button';
import { InputGroup } from '@paljs/ui/Input';
import { Checkbox } from '@paljs/ui/Checkbox';
import React, { useState } from 'react';
import { Link, navigate } from 'gatsby';

import Auth, { Group } from '../../components/Auth';
import Socials from '../../components/Auth/Socials';
import SEO from '../../components/SEO';
import { handleLogin, isLoggedIn, setUser } from '../../services/auth';

export default function Login() {
  const [username, setUsername] = useState('');
  const [password, setPassword] = useState('');
  if (isLoggedIn()) {
    navigate('/dashboard');
  }
  const onCheckbox = () => {
    // v will be true or false
  };

  const handleSubmit = (e) => {
    e.preventDefault();

    const res = handleLogin(username, password);

    res
      .then((response) => {
        if (response.DATA && response.DATA.STATUS === 'SUCCESS') {
          return setUser({
            username: response.DATA.USERNAME,
            lastLogin: response.DATA.LAST_LOGIN,
            email: response.DATA.EMAIL,
          });
        }
        console.log(response);
      })
      .then(() => navigate('/dashboard')); // waits server message with corresponding requestId: {id: 'xxx', ...};
    if (!res) {
      alert('Falsch');
    }
  };
  return (
    <Auth title="Login" subTitle="Hello! Login with your email">
      <SEO title="Login" />
      <form onSubmit={(event) => handleSubmit(event)}>
        <InputGroup fullWidth>
          <input
            value={username}
            onChange={(e) => setUsername(e.target.value)}
            type="email"
            placeholder="Email Address"
          />
        </InputGroup>
        <InputGroup fullWidth>
          <input
            value={password}
            onChange={(e) => setPassword(e.target.value)}
            type="password"
            placeholder="Password"
          />
        </InputGroup>
        <Group>
          <Checkbox onChange={onCheckbox}>Remember me</Checkbox>
          <Link to="/auth/request-password">Forgot Password?</Link>
        </Group>
        <Button status="Success" type="submit" shape="SemiRound" fullWidth>
          Login
        </Button>
      </form>
      <Socials />
      <p>
        Don&apos;t have account? <Link to="/auth/register">Register</Link>
      </p>
    </Auth>
  );
}
