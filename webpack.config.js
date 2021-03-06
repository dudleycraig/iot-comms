const webpack = require('webpack')
const path = require('path')
const HtmlWebpackPlugin = require('html-webpack-plugin')
const CleanWebpackPlugin = require('clean-webpack-plugin')
const MiniCssExtractPlugin = require('mini-css-extract-plugin')
const autoPreFixer = require('autoprefixer')

process.stdout.write('----------------------------------------------------------------\n')
process.stdout.write('---                        iot-comms                         ---\n')
process.stdout.write('---                                                          ---\n')
process.stdout.write('---    tech: es6, react, redux, jest, eslint, bootstrap      ---\n')
process.stdout.write('---   theme: https://bootswatch.com/sandstone                ---\n')
process.stdout.write('---                                                          ---\n')
process.stdout.write('----------------------------------------------------------------\n')

module.exports = {
  entry: {
    index: path.resolve(__dirname, 'src/client/index.js'),
  },
  output: {
    path: path.resolve(__dirname, 'dist'),
    filename: '[name].js',
    publicPath: '',
  },
  mode: 'development',
  target: 'web',
  watch: true,
  resolve: {
    extensions: ['*', '.js', '.jsx', '.json', '.css', '.scss'],
    modules: [
      'node_modules',
      path.resolve(__dirname, 'src/client/'),
      path.resolve(__dirname, 'vendor/'),
      path.resolve(__dirname, 'public/images/'),
    ],
  },
  devtool: 'source-map',
  plugins: [
    new webpack.ProvidePlugin({
      $: 'jquery',
      jQuery: 'jquery',
      Popper: ['popper.js', 'default'],
    }),
    new webpack.HotModuleReplacementPlugin(),
    /**
       *
    new CleanWebpackPlugin(),
    new HtmlWebpackPlugin({ 
      template: 'public/index.html',
      favicon: 'public/favicon.ico',
      minify: {
        removeComments: true,
        collapseWhitespace: true,
      },
      inject: true
    }),
       * */
    new webpack.NamedModulesPlugin(),
    new MiniCssExtractPlugin({
      filename: '[name].css',
      chunkFilename: '[id].css',
    }),
  ],
  module: {
    rules: [
      {
        test: /\.(js|jsx)$/,
        include: [path.resolve(__dirname, 'src')],
        exclude: [path.resolve(__dirname, 'node_modules')],
        use: [{ loader: 'babel-loader', query: { presets: ['stage-3', 'react'] } }],
      },
      {
        test: /\.(scss|sass)$/,
        include: [
          path.resolve(__dirname, 'node_modules/bootstrap/scss'),
          // path.resolve(__dirname, 'node_modules/font-awesome/scss'),
          path.resolve(__dirname, 'node_modules/@fortawesome/fontawesome-free/scss'),
          path.resolve(__dirname, 'src/client/styles'),
        ],
        use: [
          // { loader: MiniCssExtractPlugin.loader, options: { publicPath: '/' } },
          { loader: 'style-loader' },
          { loader: 'css-loader', options: { module: true } }, // translate css into js
          {
            loader: 'postcss-loader',
            options: {
              plugins: () => [autoPreFixer({ browsers: ['> 1%', 'last 2 versions'] })],
            },
          },
          { loader: 'sass-loader', options: { outputStyle: 'expanded' } }, // compiles sass/scss to css
        ],
      },
      {
        test: /\.(jpe?g|png|gif|ico)$/i,
        use: [{ loader: 'file-loader', options: { name: '[name].[ext]', outputPath: 'images/' } }],
      },
      {
        test: /\.(svg)(\?v=\d+\.\d+\.\d+)?$/i,
        use: [{ loader: 'file-loader', options: { name: '[name].[ext]', outputPath: 'images/' } }],
      },
      {
        test: /.(woff(2)?)(\?v=\d+\.\d+\.\d+)?$/i,
        use: [{ loader: 'file-loader', options: { name: '[name].[ext]', outputPath: 'fonts/', publicPath: 'fonts/' } }],
      },
      {
        test: /.(ttf)(\?v=\d+\.\d+\.\d+)?$/i,
        use: [{ loader: 'file-loader', options: { name: '[name].[ext]', outputPath: 'fonts/', publicPath: 'fonts/' } }],
      },
      {
        test: /.(eot)(\?v=\d+\.\d+\.\d+)?$/i,
        use: [{ loader: 'file-loader', options: { name: '[name].[ext]', outputPath: 'fonts/', publicPath: 'fonts/' } }],
      },
    ],
  },
  devServer: {
    contentBase: path.resolve(__dirname, 'public'),
    inline: true,
    open: true,
    hot: true,
    host: '0.0.0.0',
    port: 8081,
  },
}
