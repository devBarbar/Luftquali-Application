import React, { useEffect } from 'react';
import { logout } from '../services/auth';
import { navigate } from 'gatsby';
function Logout() {
  useEffect(() => {
    logout(() => navigate('/auth/login'));
  }, []);
  return <div></div>;
}

export default Logout;
