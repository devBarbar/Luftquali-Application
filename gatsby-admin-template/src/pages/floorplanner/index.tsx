import React, { useEffect } from 'react';
import Floorplanner from '../../components/floorplanner/floorplanner';
import { wsp } from '../../components/websocket/websocket';
import { InputGroup } from '@paljs/ui/Input';
import styled from 'styled-components';

const Input = styled(InputGroup)`
  margin-bottom: 10px;
`;
const Home = () => {
  return (
    <div
      style={{
        width: '100%',
        height: '100%',
      }}
    >
      <Input fullWidth size="Medium">
        <input type="text" placeholder="Size Medium" />
      </Input>
      <Floorplanner></Floorplanner>
    </div>
  );
};
export default Home;
