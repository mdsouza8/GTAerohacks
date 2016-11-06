var rp1 = radialProgress(document.getElementById("radial"))
	.diameter(200)
	.value(0)
	.render();


function updateRadial(p, s) { //get the value of k / k + na
	var val = 100 * (p/(p+s));
	rp1
		.diameter(200)
		.value(val)
		.render();
}

function clearRadial() {
	rp1
		.diameter(200)
		.value(0)
		.render();
}