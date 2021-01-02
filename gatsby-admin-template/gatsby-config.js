module.exports = {
  siteMetadata: {
    title: 'Free React Admin dashboard template based on Gatsby',
    description: 'Admin dashboard template based on Gatsby with @paljs/ui component package.',
    author: 'Ahmed Elywa',
  },
  plugins: [
    'gatsby-plugin-styled-components',
    'gatsby-plugin-typescript',
    {
      resolve: `gatsby-plugin-layout`,
      options: {
        component: require.resolve(`./src/Layouts/index.tsx`),
      },
    },
    {
      resolve: 'gatsby-source-filesystem',
      options: {
        name: 'images',
        path: `${__dirname}/src/images`,
      },
    },
    'gatsby-transformer-sharp',
    'gatsby-plugin-sharp',
    {
      resolve: 'gatsby-plugin-manifest',
      options: {
        name: 'Luftqualität Dashboard',
        short_name: 'FRAUAS',
        start_url: '/',
        display: 'minimal-ui',
        icon: 'src/images/OAH.png', // This path is relative to the root of the site.
      },
    },
    `gatsby-plugin-offline`,
  ],
};
