import React, { useEffect } from 'react';
import { Router } from '@reach/router';

import { navigate } from 'gatsby';
import Login from './auth/login';

export default function Index() {
  useEffect(() => {
    navigate('/dashboard');
  }, []);

  return (
    <Router>
      <Login path="/auth/login"></Login>
    </Router>
  );
}
