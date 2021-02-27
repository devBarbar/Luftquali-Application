import { MenuItemType } from '@paljs/ui/types';

const items: MenuItemType[] = [
  {
    title: 'Home Page',
    icon: { name: 'home' },
    link: { to: '/dashboard' },
  },
  {
    title: 'Create New Room',
    icon: { name: 'home' },
    link: { to: '/floorplanner' },
  },
  {
    title: 'Sensors',
    icon: { name: 'home' },
    link: { to: '/sensors' },
  },
];

export default items;
