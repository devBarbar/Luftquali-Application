/**
 * Implement Gatsby's Node APIs in this file.
 *
 * See: https://www.gatsbyjs.org/docs/node-apis/
 */

// You can delete this file if you're not using it
exports.onCreatePage = ({ page, actions }) => {
  const { createPage } = actions;

  if (page.path.match(/auth/)) {
    page.context.layout = 'auth';
    createPage(page);
  }
};

exports.onCreateWebpackConfig = ({ stage, rules, loaders, plugins, actions }) => {
  actions.setWebpackConfig({
    module: {
      rules: [
        {
          test: /\.(jpe?g|png|gif|mtl|obj)$/i,
          use: [
            {
              loader: 'file-loader',
              options: {
                hash: 'sha512',
                digest: 'hex',
                name: '[path][name].[ext]',
                context: 'src/components/floorplanner',
              },
            },
          ],
        },
      ],
    },
    plugins: [
      plugins.define({
        isProduction: stage !== `develop` || stage !== `develop-html`,
      }),
    ],
  });
};
