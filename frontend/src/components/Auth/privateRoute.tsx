import React from 'react';
import { isLoggedIn } from '../../services/auth';
import { navigate } from 'gatsby';

function PrivateRoute({ component: Component, location, ...rest }) {
  if (!isLoggedIn && location.pathname !== `/app/login`) {
    navigate('/auth/login');
    return null;
  }

  return <Component {...rest} />;
}

export default PrivateRoute;
