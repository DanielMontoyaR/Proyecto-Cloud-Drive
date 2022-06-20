const mongoose = require("mongoose");

const userSchema = mongoose.Schema({
  tags: {
    type: Array,
    required: true,
  },
  file: {
    type: String,
    required: true
  }
});

module.exports = mongoose.model('User', userSchema);