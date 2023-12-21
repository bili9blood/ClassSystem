/** @type {import('tailwindcss').Config} */
module.exports = {
  content: ["./src/**/*.{ts,js,tsx,jsx}"],
  theme: {
    extend: {
      colors: {
        highlight: "#ee8545"
      }
    }
  },
  plugins: [require("daisyui")]
};
