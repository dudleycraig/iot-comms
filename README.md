# iot-comms 

## installation

### clone repository

> git clone https://github.com/dudleycraig/iot-comms.git iot-comms 

### update config to reflect your environment

> vim webpack.config.js  
change key 'host:' to your dev server's domain  
```javascript
module.exports = {  
   ...  
   devServer: {  
      ...  
      host:'server-domain'  
   }  
}  
``` 

### pull node modules

> npm install

### run server in development mode

> npm start

## app structure
|  
|- package.json  
|- webpack.config.js  
|- .babelrc  
|- .eslintignore  
|- .eslintrc.json  
|- .tern-project  
|- .vimrc  
|  
|- public/  
|&nbsp;&nbsp;&nbsp;|- index.html  
|&nbsp;&nbsp;&nbsp;|- javascript/  
|&nbsp;&nbsp;&nbsp;|- images/  
|&nbsp;&nbsp;&nbsp;|- fonts/  
|&nbsp;&nbsp;&nbsp;|- css/  
|  
|- src/  
|&nbsp;&nbsp;&nbsp;|- client/  
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|- actions/  
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|- components/  
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|- containers/  
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|- reducers/  
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|- styles/  
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|- types/  
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|- index.js  
|&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|- store.js  

## workflow

console based, event driven

## issues
