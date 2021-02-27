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
          alert('E-Mail bereits registriert');
          return;
        }
        navigate('/auth/login');

        console.log(result);
      });
  };
  return (
    <Auth title="Account erstellen">
      <SEO title="Register" />
      <form onSubmit={handleSubmit}>
        <Input fullWidth>
          <input
            value={username}
            onChange={(e) => setUsername(e.target.value)}
            type="text"
            placeholder="Benutzername"
          />
        </Input>
        <Input fullWidth>
          <input value={email} onChange={(e) => setEmail(e.target.value)} type="email" placeholder="E-Mail Adresse" />
        </Input>
        <Input fullWidth>
          <input
            value={password}
            onChange={(e) => setPassword(e.target.value)}
            type="password"
            placeholder="Passwort"
          />
        </Input>
        <Input fullWidth>
          <input
            value={passwordConfirmation}
            onChange={(e) => setPasswordConfirmation(e.target.value)}
            type="password"
            placeholder="Passwort bestätigen"
          />
        </Input>
        <Button status="Success" type="submit" shape="SemiRound" fullWidth>
          Registrieren
        </Button>
      </form>
      <p>
        Sie besitzten bereits einen Zugang? <Link to="/auth/login">Log In</Link>
      </p>
    </Auth>
  );
}
