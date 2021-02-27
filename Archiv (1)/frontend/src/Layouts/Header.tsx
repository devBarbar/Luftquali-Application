import React, { useEffect, useState } from 'react';
import { Link } from 'gatsby';
import styled, { DefaultTheme } from 'styled-components';
import Select from '@paljs/ui/Select';
import { LayoutHeader } from '@paljs/ui/Layout';
import { EvaIcon } from '@paljs/ui/Icon';
import { Button } from '@paljs/ui/Button';
import { Actions } from '@paljs/ui/Actions';
import ContextMenu from '@paljs/ui/ContextMenu';
import User from '@paljs/ui/User';
import { getPathReady } from './Sidebar';
import { Location } from '@reach/router';
import { breakpointDown } from '@paljs/ui/breakpoints';
import { getUser } from '../services/auth';
import { wsp } from '../components/websocket/websocket';

const HeaderStyle = styled.div`
  display: flex;
  width: 100%;
  justify-content: space-between;
  ${breakpointDown('sm')`
    .right{
      display: none;
    }
  `}
  .right > div {
    height: auto;
    display: flex;
    align-content: center;
  }
  .logo {
    font-size: 1.25rem;
    white-space: nowrap;
    text-decoration: none;
  }
  .left {
    display: flex;
    align-items: center;
    .github {
      font-size: 18px;
      margin-right: 5px;
    }
  }
`;

const Label = styled.span`
  display: flex;
  align-items: center;
`;

const SelectStyled = styled(Select)`
  min-width: 150px;
`;

interface HeaderProps {
  toggleSidebar: () => void;
  theme: {
    set: (value: DefaultTheme['name']) => void;
    value: DefaultTheme['name'];
  };
  changeDir: () => void;
  dir: 'rtl' | 'ltr';
}

const Header: React.FC<HeaderProps> = (props) => {
  const [alert, setAlert] = useState([]);
  useEffect(() => {
    wsp
      .open()
      .then(() => wsp.sendRequest({ MSG_TYPE: 'GET_ALERTS' })) // actually sends {foo: 'bar', id: 'xxx'}, because `attachRequestId` defined above
      .then((response) => setAlert(response.DATA)); // waits server message with corresponding requestId: {id: 'xxx', ...}
  }, []);
  const user = getUser();
  console.log(user);
  return (
    <LayoutHeader fixed>
      <HeaderStyle>
        <Actions
          size="Medium"
          actions={[
            {
              icon: { name: 'menu-2-outline' },
              url: {
                onClick: props.toggleSidebar,
              },
            },
            {
              content: (
                <Link to="/" className="logo">
                  Airquality Monitoring
                </Link>
              ),
            },
          ]}
        />
        {alert.map((val) => (
          <span>{val} Überprüfen</span>
        ))}
        <Actions
          size="Small"
          className="right"
          actions={[
            {
              content: (
                <Location>
                  {({ location }) => (
                    <ContextMenu
                      style={{ cursor: 'pointer' }}
                      placement="bottom"
                      currentPath={getPathReady(location.pathname)}
                      items={[
                        { title: 'Einstellungen', link: { to: '/settings/thresholds' } },
                        { title: 'Log out', link: { to: '/logout' } },
                      ]}
                      Link={Link}
                    >
                      <User
                        image="url('/icons/icon-72x72.png')"
                        name={user && user.username}
                        title={user.email}
                        size="Medium"
                      />
                    </ContextMenu>
                  )}
                </Location>
              ),
            },
          ]}
        />
      </HeaderStyle>
    </LayoutHeader>
  );
};
export default Header;
