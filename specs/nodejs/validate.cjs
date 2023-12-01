'use strict'

const validate = require('validate-npm-package-name');

const result = validate(process.env.NAME);
if (result.errors && result.errors.length > 0) {
  console.log('');
  for (const e of result.errors) {
    console.log('\x1b[31m%s\x1b[0m: %s', 'ERROR', e);
  }
  process.exit(1);
}
