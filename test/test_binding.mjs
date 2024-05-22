import assert from "assert";
import { split } from "../dist/binding.js";

function testBasic() {
  const result = split();
  console.log('result:', result);
  assert.strictEqual(result, "ok", "Unexpected value returned");
}

assert.doesNotThrow(testBasic, undefined, "testBasic threw an expection");

console.log("Tests passed- everything looks OK!");
