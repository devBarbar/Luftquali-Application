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
      .then(() => navigate('/dashboard'));
    if (!res) {
      alert('Falsch');
    }
  };
  return (
    <Auth
      title="Login"
      subTitle="Hallo, um Zugriff auf das Dashboard zu bekommen, loggen Sie sich bitte in ihren Account ein"
    >
      <SEO title="Login" />
      <form onSubmit={(event) => handleSubmit(event)}>
        <InputGroup fullWidth>
          <input
            value={username}
            onChange={(e) => setUsername(e.target.value)}
            type="email"
            placeholder="E-Mail Adresse"
          />
        </InputGroup>
        <InputGroup fullWidth>
          <input
            value={password}
            onChange={(e) => setPassword(e.target.value)}
            type="password"
            placeholder="Passwort"
          />
        </InputGroup>
        <Button status="Success" type="submit" shape="SemiRound" fullWidth>
          Login
        </Button>
      </form>
      <p>
        Kein Account vorhanden? <Link to="/auth/register">Registrieren</Link>
      </p>
    </Auth>
  );
}
