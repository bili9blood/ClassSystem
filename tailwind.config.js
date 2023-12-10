/** @type {import('tailwindcss').Config} */
module.exports = {
  content: ["./src/**/*.{ts,js,tsx,jsx}"],
  theme: {
    extend: {
      colors: { "primary-text": "#edf5e0" }
    }
  },
  plugins: [require("daisyui")]
};
