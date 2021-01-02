import { Button } from '@paljs/ui/Button';
import { InputGroup } from '@paljs/ui/Input';
import { Checkbox } from '@paljs/ui/Checkbox';
import React, { useState } from 'react';
import styled from 'styled-components';
import { Link, navigate } from 'gatsby';

import Auth from '../../components/Auth';
import SEO from '../../components/SEO';
import Socials from '../../components/Auth/Socials';
import { wsp } from '../../components/websocket/websocket';
import { handleLogin } from '../../services/auth';

const Input = styled(InputGroup)`
  margin-bottom: 2rem;
`;

export default function Register() {
  const [username, setUsername] = useState('');
  const [password, setPassword] = useState('');
  const [email, setEmail] = useState('');
  const [passwordConfirmation, setPasswordConfirmation] = useState('');

  const handleSubmit = (e) => {
    e.preventDefault();
    wsp
      .open()
      .then(() => wsp.sendRequest({ MSG_TYPE: 'REGISTER', EMAIL: email, PASSWORD: password, USERNAME: username }))
      .then((result) => {
        console.log(result);
        if (result.DATA.STATUS == 'DUPLICATE') {
          alert('Email ADRESsse bereits registriert');
          return;
        }
        navigate('/auth/login');

        console.log(result);
      });
  };
  const onCheckbox = () => {
    // v will be true or false
  };
  return (
    <Auth title="Create new account">
      <SEO title="Register" />
      <form onSubmit={handleSubmit}>
        <Input fullWidth>
          <input value={username} onChange={(e) => setUsername(e.target.value)} type="text" placeholder="Username" />
        </Input>
        <Input fullWidth>
          <input value={email} onChange={(e) => setEmail(e.target.value)} type="email" placeholder="Email Address" />
        </Input>
        <Input fullWidth>
          <input
            value={password}
            onChange={(e) => setPassword(e.target.value)}
            type="password"
            placeholder="Password"
          />
        </Input>
        <Input fullWidth>
          <input
            value={passwordConfirmation}
            onChange={(e) => setPasswordConfirmation(e.target.value)}
            type="password"
            placeholder="Confirm Password"
          />
        </Input>
        <Checkbox onChange={onCheckbox}>
          Agree to <Link to="/">Terms & Conditions</Link>
        </Checkbox>
        <Button status="Success" type="submit" shape="SemiRound" fullWidth>
          Register
        </Button>
      </form>
      <Socials />
      <p>
        Already have an account? <Link to="/auth/login">Log In</Link>
      </p>
    </Auth>
  );
}
